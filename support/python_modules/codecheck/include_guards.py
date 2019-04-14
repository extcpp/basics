from codecheck import logger as log
import re
import sys

from .common import AccessType, Status
from .common import Operation, OperationState, AccessType

##from .common import OperationResult
##class IncludeGuardResult(OperationResult):
##    pass

g_guard_re= re.compile('#define (?P<guard>OBI_.*(HPP|H))')

class IncludeGuardState(OperationState):
    def __init__(self, access):
        super(IncludeGuardState, self).__init__(access) #creates a copy of access

class IncludeGuard(Operation):
    def __init__(self, op):
        super(IncludeGuard, self).__init__(op, "IncludeGuard")
        self.file_types = ( "h", "hpp" ) # must be tuple
        self.do_log = False
        self.mark_start = True

    @classmethod
    def State(cls, *args, **kwags):
        return IncludeGuardState(*args, **kwags)

    #def do_line(self, line, cnt, full_path, project_path, target_file_handle, state)
    #def do(self, full_path, project_path, target_file_handle, state)

    def check(self, full_path, project_path, state):
        if self.mark_start:
            log.info(project_path)
        if self.do_log:
            log.info("check ====================================================")
        state.found = False
        state.lineno = 0
        state.fix = False
        state.abort = False
        state.status = None
        state.starting = True

        ## hpp h to header
        guard = project_path.replace('include/obi','obi')
        guard = guard.replace('.','_').replace('/','_').upper()
        state.guard = guard

    def check_line(self, line, cnt, full_path, project_path, state):
        if state.found or state.abort:
            return

        if state.starting and (line.startswith('//') or line == "\n"):
            state.lineno = cnt
        else:
            state.starting = False


        if line.startswith('#'):
            if line == "#pragma once\n":
                state.line = cnt

            match = g_guard_re.match(line)
            if match:
                state.found = True
                state.line = cnt
                if match['guard'] == state.guard:
                    state.abort = True
                    state.access = []
                else:


    def modify(self, full_path, project_path, target_file_handle, state):
        if self.do_log:
            log.info("modify ===================================================")


    def modify_line(self, line, cnt, full_path, project_path, target_file_handle, state):
        #if do_log:
        #    log.info("modify line ==============================================")
        #out = target_file_handle
        if state.abort:
            return

        out = sys.stdout

        if state.lineno == cnt and state.fix == False:
            log.info("insert new gurad")
            out.write(line)
            out.write("#ifndef {}\n".format(state.guard))
            out.write("#define {}\n".format(state.guard))
            return Status.OK
        if state.lineno == cnt and state.fix == False:
            log.info("fix existing gurad")
            guard = project_path
            out.write("#ifndef {}\n".format(state.guard))
            out.write("#define {}\n")
            return Status.OK

        elif cnt == "EOF" and state.fix == False:
            # insert `#endif` for new guard
            out.write("#eindif\n")
            return Status.OK_REPLACED
        else:
            #just copy rest of file
            #out.write(line)
            return Status.OK

