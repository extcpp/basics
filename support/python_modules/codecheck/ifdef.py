import re
import sys
from pathlib import Path
from typing import Dict, Tuple, Sequence, List, IO

from obi.util.path_helper import remove_from_front, change_ext
from . import logger as log
from .common import AccessType, Status
from .common import Operation, OperationState, AccessType

## // IfDef - 2014 - Jan Christoph Uhde <Jan@UhdeJC.com>
g_ifdef_re = re.compile(r'^#[ \t]*(if(n?def)?)[ \t]+(?P<if_value>.*)$')
g_endif_re = re.compile(r'^#[ \t]*endif(?P<endif_space>[ \t])*(?P<endif_value>.*)$')

class IfDefState(OperationState):
    def __init__(self, access):
        super(IfDefState, self).__init__(access) #creates a copy of access

class IfDef(Operation):
    def __init__(self, op):
        super(IfDef, self).__init__(op, "IfDef")
        self.dry_run = False
        self.file_types = ( ".h", ".hpp", ".c", ".cpp", ".cc" ) # must be tuple
        self.do_log = False
        self.mark_start = True

    @classmethod
    def State(cls, *args, **kwags):
        return IfDefState(*args, **kwags)

    #def do_line(self, line, cnt, full_path, project_path, target_file_handle, state)
    #def do(self, full_path, project_path, target_file_handle, state)

    def check(self, full_path: Path, project_path: Path, state: OperationState) -> Status:
        state.replacements = []
        state.stack = [] # used for matchinga
        return Status.OK

    def check_line(self, line: str, cnt, full_path: Path, project_path: Path, state: OperationState):
        match = g_ifdef_re.match(line)
        if match:
            state.stack.append((cnt, match['if_value']))
            return Status.OK

        match = g_endif_re.match(line)
        if match:
            if_value_pair = state.stack.pop()
            space = match['endif_space']
            if not space:
                space = ' '
            target_endif_value = r'#endif{}// {}{}'.format(space ,if_value_pair[1],"\n")
            if not target_endif_value == line:
                state.replacements.append((cnt, target_endif_value))


        return Status.OK

    def modify(self, full_path: Path, project_path: Path, target_file_handle: IO, state: OperationState):
        if not state.replacements:
            return Status.OK_SKIP_FILE
        else:
            state.replacements.reverse()
            return Status.OK

    def modify_line(self, line: str, cnt, full_path: Path, project_path: Path, target_file_handle: IO, state: IfDefState):
        out = target_file_handle
        if cnt == 'EOF':
            return Status.OK_REPLACED
        else:
            if state.replacements:
                pair = state.replacements[-1]
                if pair[0] == cnt:
                    out.write(pair[1])
                    state.replacements.pop()
                    return Status.OK

        out.write(line)
        return Status.OK
