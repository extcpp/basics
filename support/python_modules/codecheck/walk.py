#!/usr/bin/python3

from codecheck import logger as log
from codecheck.common import *
from codecheck.state import StateAndConfiguration

def handle_file(full_path, state_and_config):
    #log.info("--------------------------------> handle file")
    operation = state_and_config.current_operation
    conf = state_and_config.configuration
    project_path = os.path.relpath(full_path, conf.project_root)

    state = operation.new_state()
    status = Status.OK

    while state.access:
        access = state.access.pop()
        if access == AccessType.MODIFY:
            target_file = full_path + ".replacement"
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

def do_operation(full_path, project_path, target_file_handle, state_and_config):
    operation = state_and_config.current_operation
    state = state_and_config.state.get(operation.name, dict())

    status = Status.OK
    operation.do(full_path, project_path, target_file_handle, state_and_config)
    with open(full_path) as source_file_handle:
        for cnt, line in enumerate(source_file_handle):
            operation.do_line(line, cnt, full_path, project_path, target_file_handle, state_and_config)
        operation.do_line("", "EOF", full_path, project_path, target_file_handle, state_and_config)

    return status


def check_modify_source(directories_to_include, directories_or_files_to_exclude, state_and_config):
    conf = state_and_config.configuration

    include = [ conf.project_root + os.path.sep + d for d in directories_to_include ]
    exclude = [ conf.project_root + os.path.sep + d for d in directories_or_files_to_exclude ]

    for operation in conf.operations:
        log.info(operation.name)
        state_and_config.current_operation = operation

        for root, dirs, files in os.walk(conf.project_root):
            status = Status.OK

            dirs[:] = [ d for d in dirs if (root + os.path.sep + d).startswith(tuple(include)) ]
            if root in exclude:
                continue

            for filename in files:
                full_path = root + os.path.sep + filename

                if full_path in exclude:
                    continue

                if (filename.endswith(operation.file_types)):
                    status = handle_file(full_path, state_and_config)

                if not Status.is_good(status):
                    return status

    return 0

