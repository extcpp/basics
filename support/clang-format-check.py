#!/usr/bin/env python3
#Copyright - 2019 - Jan Christoph Uhde <Jan@UhdeJC.com>

# FIXME - unfinished - not worth the trouble at the moment

import os, sys, re
from pathlib import Path
from subprocess import Popen, PIPE, STDOUT

python_dir = Path(__file__).parent.resolve()
modules_dir = python_dir.joinpath("python_modules")
sys.path.insert(0,str(modules_dir)) #add modules dir

from codecheck import logger #get and init logging - must be frist of our imports
from codecheck.util import find_clang_format
from obi.util.path_helper import apply_action_to_files, filter_cpp, create_filter_path

project_dir = Path(__file__).resolve().parent.parent

def check_diff():
    clang_format = find_clang_format(9.0)

    if not clang_format:
        logger.fatal("no matching clang-format found")
        sys.exit(1)

    #TODO diff
    #TODO call clang-format-diff
    #TODO colordiff
    return 1


if __name__ == "__main__":
    sys.exit(check_diff())
