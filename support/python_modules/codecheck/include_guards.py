import re
from codecheck.common import Operation, AccessType

class IncludeGurad(Operation):
    def __init__(self, op):
        super(IncludeGurad, self).__init__(op, "IncludeGurad")
        self.file_types = ( "h", "hpp" ) # must be tuple
        self.do_log = True

    #def do_line(self, line, cnt, full_path, project_path, target_file_handle, state)
    #def do(self, full_path, project_path, target_file_handle, state)

    def check_line(self, line, cnt, full_path, project_path, state):
        pass

    def modify_line(self, line, cnt, full_path, project_path, target_file_handle, state):
        pass

    def check(self, full_path, project_path, state):
        pass

    def modify(self, full_path, project_path, target_file_handle, state):
        pass
