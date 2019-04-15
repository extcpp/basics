#!/usr/bin/python3

from codecheck.common import *
from codecheck.state import StateAndConfiguration

def handle_file(full_path: Path, state_and_config: StateAndConfiguration):
    #log.info("--------------------------------> handle file")
    operation = state_and_config.current_operation
    conf = state_and_config.configuration
    project_path = full_path.relative_to(conf.project_root)
    project_path.parts
    full_path.parts

    state = operation.new_state()
    status = Status.OK

    while state.access:
        access = state.access.pop()
        if access == AccessType.MODIFY:
            target_file = change_ext(full_path, ".replacement")
            with open(target_file, 'w') as target_file_handle:
                status = do_operation(full_path, project_path, target_file_handle, state_and_config)

            if status == Status.OK_REPLACED:
                os.rename(target_file, full_path)
            else:
                os.unlink(target_file)

        elif access == AccessType.READ:
            #not modifying operation
            status = do_operation(full_path, project_path, None, state_and_config)

        if not Status.is_good(status):
            #log.info("<-------------------------------- handle file " + status.name)
            return status # filter OK_REPLACED

    #log.info("<-------------------------------- handle file " + status.name)
    status = Status.good_to_ok(status)
    return status # filter OK_REPLACED

def do_operation(full_path: Path, project_path: Path, target_file_handle: IO, state_and_config: StateAndConfiguration):
    full_path.parts
    project_path.parts
    operation = state_and_config.current_operation
    state = state_and_config.state.get(operation.name, dict())

    status = Status.OK
    operation.do(full_path, project_path, target_file_handle, state_and_config)
    with open(full_path) as source_file_handle:
        for cnt, line in enumerate(source_file_handle):
            operation.do_line(line, cnt, full_path, project_path, target_file_handle, state_and_config)
        operation.do_line("", "EOF", full_path, project_path, target_file_handle, state_and_config)

    return status


def check_modify_source(directories_to_include, directories_or_files_to_exclude, state_and_config :StateAndConfiguration):
    conf = state_and_config.configuration

    print(conf.project_root)

    include = [ str(conf.project_root.joinpath(d)) for d in directories_to_include ]
    exclude = [ str(conf.project_root.joinpath(d)) for d in directories_or_files_to_exclude ]

    for operation in conf.operations:
        log.info(operation.name)
        state_and_config.current_operation = operation

        for root, dirs, files in os.walk(conf.project_root):
            status = Status.OK

            dirs[:] = [ d for d in dirs if (str(root) + os.path.sep + d).startswith(tuple(include)) ]
            if root in exclude:
                continue

            for filename in files:
                filename = Path(filename)
                full_path = Path(root).joinpath(filename)
                #full_path.parts # -- assert that we have a real pathlib Path

                if str(full_path) in exclude:
                    continue

                if (filename.suffix in operation.file_types):
                    status = handle_file(full_path, state_and_config)

                if not Status.is_good(status):
                    return status

    return 0

