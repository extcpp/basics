import sys
import os
from enum import Enum
from codecheck import logger as log

class AccessType(Enum):
    READ = 1
    MODIFY = 2

class Operation():
    def __init__(self, op_type, name):
        log.info("create {} with {}".format(name, op_type))
        self.name = name
        self.do_log = False
        self.do_log_detail = False
        self.access = [  ]

        if op_type == None:
            pass
        elif op_type == AccessType.MODIFY:
            self.access.append( AccessType.READ )
            self.access.append( AccessType.MODIFY )
        elif op_type == AccessType.READ:
            self.access.append( AccessType.READ )

    def do_line(self, line, cnt, full_path, project_path, target_file_handle, state):
        if self.do_log_detail:
            l = log.info
            l("{} {}".format(cnt,line))

        if target_file_handle:
            self.modify_line(line, cnt, full_path, project_path, target_file_handle, state)
        else:
            self.check_line(line, cnt, full_path, project_path, state)


    def do(self, full_path, project_path, target_file_handle, state):
        if self.do_log:
            l = log.info
            l("------")
            l("{}".format(self.name))
            #l("{}".format(full_path))
            l("{}".format(project_path))

        if target_file_handle:
            self.modify(full_path, project_path, target_file_handle, state)
        else:
            self.check(full_path, project_path, state)

class Status(Enum):
    OK = 0
    OK_REPLACED = 1
    FAIL = 2

def is_good(status):
    return status in (Status.OK, Status.OK_REPLACED)
