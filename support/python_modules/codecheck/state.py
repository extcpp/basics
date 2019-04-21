import sys
import os

from .common import AccessType
from .include_guards import IncludeGuard
from .copyright import Copyright

from enum import Enum

class Configuration():
    def __init__(self, project_root, check_only):
        access = AccessType.READ
        if not check_only:
            access = AccessType.MODIFY

        self.project_root = project_root
        self.fix_include_guards = access
        self.fix_copyright = access
        self.create_operation_list()

    def create_operation_list(self):
        self.operations = []
        if self.fix_include_guards:
            self.operations.append(IncludeGuard(self.fix_include_guards))
        if self.fix_copyright:
            self.operations.append(Copyright(self.fix_copyright))

class StateAndConfiguration():
    def __init__(self, project_root, check_only):
        self.configuration = Configuration(project_root, check_only)
        self.state = dict() #map { name_of_operation : some_state }
        self.reset()

    def reset(self):
        self.current_operation = None
        self.current_access = None
        self.current_file = None
