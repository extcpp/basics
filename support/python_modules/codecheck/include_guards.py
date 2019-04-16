import re
import sys
from pathlib import Path
from typing import Dict, Tuple, Sequence, List, IO

from obi.util.path_helper import remove_from_front, change_ext
from . import logger as log
from .common import AccessType, Status
from .common import Operation, OperationState, AccessType

g_guard_re = re.compile(r'#ifndef\s+_?(?P<guard>OBI_.*(HEADER|HPP|H))')

class IncludeGuardState(OperationState):
    def __init__(self, access):
        super(IncludeGuardState, self).__init__(access) #creates a copy of access

class IncludeGuard(Operation):
    def __init__(self, op):
        super(IncludeGuard, self).__init__(op, "IncludeGuard")
        self.dry_run = False
        self.file_types = ( ".h", ".hpp" ) # must be tuple
        self.do_log = False
        self.mark_start = True

    @classmethod
    def State(cls, *args, **kwags):
        return IncludeGuardState(*args, **kwags)

    #def do_line(self, line, cnt, full_path, project_path, target_file_handle, state)
    #def do(self, full_path, project_path, target_file_handle, state)

    def check(self, full_path: Path, project_path: Path, state: OperationState) -> Status:
        state.line_for_infdef = 0
        state.starting = True
        state.insert_new = True # else fix old

        ## hpp h to header
        path = project_path
        if path.parts[1] == 'obi':
            path = remove_from_front(project_path, 'include')
        path = change_ext(path,'_HEADER')
        guard = "_".join(path.parts).upper()
        state.guard = guard

        return Status.OK

    def check_line(self, line: str, cnt, full_path: Path, project_path: Path, state: OperationState):
        if state.starting and (line.startswith('//') or line == "\n"):
            state.line_for_infdef = cnt
        else:
            state.starting = False

        if line.startswith('#'):
            if line.startswith("#pragma once") and state.insert_new:
                state.line_for_infdef = cnt  # insert after prama once

            match = g_guard_re.search(line)

            if match:
                state.line_for_infdef = cnt
                if match['guard'] == state.guard:
                    state.access = []
                    return Status.OK_SKIP_FILE
                else:
                    state.insert_new = False #do not insert but fix
                    return Status.OK_SKIP_LINEWISE_ACCESS

        return Status.OK

    def modify(self, full_path: Path, project_path: Path, target_file_handle: IO, state: OperationState):
        return Status.OK

    def modify_line(self, line: str, cnt, full_path: Path, project_path: Path, target_file_handle: IO, state: IncludeGuardState):
        out = target_file_handle

        ## insert new
        if state.insert_new:
            if state.line_for_infdef == cnt:
                log.info("insert new gurad in")
                out.write(line)
                out.write("#ifndef {}\n".format(state.guard))
                out.write("#define {}\n".format(state.guard))
                return Status.OK
            elif cnt == "EOF":
                out.write("#endif // {}".format(state.guard))
                return Status.OK_REPLACED

        ## fix old
        if not state.insert_new:
            if state.line_for_infdef == cnt:
                out.write("#ifndef {}\n".format(state.guard))
                out.write("#define {}\n".format(state.guard))
                return Status.OK
            elif state.line_for_infdef + 1 == cnt:
                return Status.OK
            elif cnt == "EOF":
                return Status.OK_REPLACED


        #just copy rest of file
        out.write(line)
        return Status.OK

