import sys
import os
from typing import Dict, Tuple, Sequence, List, IO
from enum import Enum
from pathlib import Path

from obi.util.path_helper import remove_from_front, change_ext

from . import logger as log

class AccessType(Enum):
    READ = 1
    MODIFY = 2

class Status(Enum):
    OK = 0
    OK_REPLACED = 1
    FAIL = 2

    @classmethod
    def is_good(cls, status):
        return status in (Status.OK, Status.OK_REPLACED)

    @classmethod
    def good_to_ok(cls, status):
        if cls.is_good(status):
            return Status.OK
        else:
            return status

class OperationState():
    def __init__(self, access):
        self.access = list(access)

#class OperationResult():
#    def __init__(self):
#        self.status = Status.OK
#        self.state = None

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
            self.access.append( AccessType.MODIFY )
            self.access.append( AccessType.READ )
        elif op_type == AccessType.READ:
            self.access.append( AccessType.READ )

    def do_line(self, line, cnt, full_path: Path, project_path: Path, target_file_handle: IO, state):
        if self.do_log_detail:
            l = log.info
            l("{} {}".format(cnt,line))

        if target_file_handle:
            return self.modify_line(line, cnt, full_path, project_path, target_file_handle, state)
        else:
            return self.check_line(line, cnt, full_path, project_path, state)

    def do(self, full_path: Path, project_path: Path, target_file_handle: IO, state):
        if self.do_log:
            l = log.info
            l("{}".format(self.name))
            l("{}".format(project_path))

        if target_file_handle:
            return self.modify(full_path, project_path, target_file_handle, state)
        else:
            return self.check(full_path, project_path, state)

    def new_state(self):
        state = self.State(self.access)
        return state
