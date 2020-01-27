import re
import sys
from pathlib import Path
from typing import Dict, Tuple, Sequence, List, IO

from obi.util.path_helper import remove_from_front, change_ext
from . import logger as log
from .common import AccessType, Status
from .common import Operation, OperationState, AccessType

## // Copyright - 2014 - Jan Christoph Uhde <Jan@UhdeJC.com>
g_copy_re = re.compile(r'//.*Copyright.*Jan Christoph Uhde')
g_copy_exact_re = re.compile(r'// Copyright - (20\d{2}(-20\d{2})?) - Jan Christoph Uhde <Jan@UhdeJC.com>')
g_copy_format = ( "// Copyright - {} - Jan Christoph Uhde <Jan@UhdeJC.com>\n" +
                  "// Please see LICENSE.md for license or visit https://github.com/extcpp/basics\n" )

class CopyrightState(OperationState):
    def __init__(self, access):
        super(CopyrightState, self).__init__(access) #creates a copy of access

class Copyright(Operation):
    def __init__(self, op):
        super(Copyright, self).__init__(op, "Copyright")
        self.dry_run = False
        self.file_types = ( ".h", ".hpp", ".c", ".cpp", ".cc" ) # must be tuple
        self.do_log = False
        self.mark_start = True

    @classmethod
    def State(cls, *args, **kwags):
        return CopyrightState(*args, **kwags)

    #def do_line(self, line, cnt, full_path, project_path, target_file_handle, state)
    #def do(self, full_path, project_path, target_file_handle, state)

    def check(self, full_path: Path, project_path: Path, state: OperationState) -> Status:
        state.line_for_copyright = 0
        state.insert_new = True # else fix old
        return Status.OK

    def check_line(self, line: str, cnt, full_path: Path, project_path: Path, state: OperationState):
        if g_copy_re.search(line):
            state.line_for_copyright = cnt
            state.insert_new = False

            if g_copy_exact_re.match(line):
                state.access = []
                return Status.OK_SKIP_FILE
            else:
                return Status.OK_SKIP_LINEWISE_ACCESS

        return Status.OK

    def modify(self, full_path: Path, project_path: Path, target_file_handle: IO, state: OperationState):
        out = target_file_handle
        if state.insert_new:
            log.info("insert {}".format(str(state.insert_new)))
            out.write(g_copy_format.format("2020"))

            with open(full_path, 'r') as infile:
                out.write(infile.read())

            return Status.OK_REPLACED

        return Status.OK

    def modify_line(self, line: str, cnt, full_path: Path, project_path: Path, target_file_handle: IO, state: CopyrightState):
        assert state.insert_new == False
        out = target_file_handle

        #need to fix
        if cnt == state.line_for_copyright:
            out.write(g_copy_format.format("xxxx-2020"))
            return Status.OK
        elif cnt == "EOF":
                return Status.OK_REPLACED

        #just copy rest of file
        out.write(line)
        return Status.OK
