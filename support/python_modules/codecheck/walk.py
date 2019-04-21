#!/usr/bin/python3

from codecheck.common import *
from codecheck.state import StateAndConfiguration

def handle_file(full_path: Path, state_and_config: StateAndConfiguration):
    conf = state_and_config.configuration
    project_path = full_path.relative_to(conf.project_root)

    log.info("handle file {}".format(project_path) )
    log.debug("---> " + str(full_path))

    operation = state_and_config.current_operation

    #assert that we have path objects
    assert project_path.parts
    assert full_path.parts

    state = operation.new_state()
    status = Status.OK

    while state.access:
        access = state.access.pop()

        if access == AccessType.MODIFY:
            target_file = full_path.parent.joinpath(full_path.name + ".replacement")

            if operation.dry_run:
                log.info("dryrun on target {}".format(target_file))
                status = do_operation(full_path, project_path, sys.stdout, state_and_config, state)
                assert status.name
            else:
                with open(target_file, 'w') as target_file_handle:
                    status = do_operation(full_path, project_path, target_file_handle, state_and_config, state)
                    assert status.name

                if status == Status.OK_REPLACED:
                    log.info("replace {}".format(project_path))
                    os.rename(target_file, full_path)
                    break
                else:
                    os.unlink(target_file)

        elif access == AccessType.READ:
            status = do_operation(full_path, project_path, None, state_and_config, state)
            assert status.name

        else:
            pass

        if Status.is_done(status):
            break

        if not Status.is_good(status):
            break

    log.debug("<--- handle file " + status.name)
    return Status.good_to_ok(status)

def do_operation(full_path: Path, project_path: Path, target_file_handle: IO, state_and_config: StateAndConfiguration, state) -> Status:
    assert full_path.parts
    assert project_path.parts
    operation = state_and_config.current_operation

    status = operation.do(full_path, project_path, target_file_handle, state)
    assert status.name

    if not Status.is_good(status) or Status.is_done(status):
        return status

    with open(full_path) as source_file_handle:
        for cnt, line in enumerate(source_file_handle):
            if Status.is_done(status, True): #done linewise
                break

            status = operation.do_line(line, cnt, full_path, project_path, target_file_handle, state)
            assert status.name

            if Status.is_done(status, True): #done linewise
                return status

        status = operation.do_line("", "EOF", full_path, project_path, target_file_handle, state)
        assert status.name

        if status == None:
            raise ValueError("status can not be None")

    return status

def to_full_path(root, path):
    return Path(root).joinpath(path)

def to_full_path_string(root, path):
    return str(to_full_path(root, path))

def should_include_path(root, path, include):
    full_path = to_full_path_string(root, path)

    if full_path.startswith(tuple(include)):
        return True

    for i in include:
        if i.startswith(full_path):
            return True

    return False



def check_modify_source(directories_to_include, directories_or_files_to_exclude, state_and_config :StateAndConfiguration):
    conf = state_and_config.configuration

    ## NOTE:
    ## project_root and root are NOT the same

    include = [ str(conf.project_root.joinpath(d)) for d in directories_to_include ]
    exclude = [ str(conf.project_root.joinpath(d)) for d in directories_or_files_to_exclude ]
    log.debug("to include: {} ".format(include))
    log.debug("to exclude: {} ".format(exclude))

    for operation in conf.operations:
        log.info(operation.name)
        state_and_config.current_operation = operation

        for root, dirs, files in os.walk(conf.project_root):
            status = Status.OK

            log.debug("dirs in: {} ".format(dirs))
            dirs[:] = [ d for d in dirs if should_include_path(root, d, include) ]
            log.debug("dirs out: {} ".format(dirs))

            if root in exclude:
                continue

            for filename in files:
                filename = Path(filename)
                full_path = to_full_path(root, filename)
                #full_path.parts # -- assert that we have a real pathlib Path

                if str(full_path) in exclude:
                    continue

                if (filename.suffix in operation.file_types):
                    status = handle_file(full_path, state_and_config)
                    assert status.name

                if not Status.is_good(status):
                    return status

    return 0

