#!/usr/bin/env python3

import os, sys, re
import logging as log

from pathlib import Path
from subprocess import Popen, PIPE, STDOUT
from pprint import pprint as PP

def find_files(path, action, *filters):
    for root, dirs, files in os.walk(path.resolve()):
        for filename in files:
            file_path=Path(root, filename)

            allow = True
            for filter in filters:
                if not (filter(file_path)):
                    allow = False
                    break

            if allow:
                action(file_path.resolve())

def filter_id(path):
    return True

def filter_path(*paths):
    def filter(path: Path):
        absolute_path = path.resolve()
        log.error("path to check: {}".format(str(absolute_path)))
        for prefix in paths:
            absolute_prefix_path = Path(prefix).resolve()
            log.error("path to check: {}".format(str(absolute_prefix_path)))
            if str(absolute_path).startswith(str(absolute_prefix_path)):
                return True
        return False
    return filter

def filter_cpp(path):
    if path.suffix in [ ".cpp", ".hpp" ]:
        return True
    else:
        return False

def find_clang_format(version = None):
    candidates = [
        "clang-format-ext",
        "clang-format-9",
        "clang-format"
    ]

    for candidate in candidates:
        candidate_path = Path(candidate)
        log.debug("testing candidate {}".format(candidate_path))

        phandle = None
        try:
            phandle = Popen(
                [ candidate_path, '--version', ],
                stdout = PIPE, stderr = STDOUT,
                universal_newlines=True
            )
        except FileNotFoundError as e:
            continue

        version_re = re.compile("version {}".format(version))

        stdout = []
        while phandle.poll() == None:
            out_err = phandle.communicate(timeout = 1)
            stdout += [ out_err[0] ]

        out=" ".join(stdout).replace('\n', ' ')

        if phandle.returncode == 0:
            print(out)
            match = version_re.search(out)
            if match:
                return candidate_path

    return None


def action_format(path: Path):
    clang_format = find_clang_format(9.0)

    if not clang_format:
        log.fatal("no matching clang-format found")
        sys.exit(1)

    command = [
        clang_format,
        "-style=file",
        str(path)
    ]

    log.error(command)
    Popen(command).wait()

path = Path(os.curdir)

project_dir = Path(__file__).resolve().parent.parent
find_files(project_dir,
           action_format,
           filter_path(Path(project_dir, "include"),
                       Path(project_dir, "tests"),
                       Path(project_dir, "examples")
                      )
           ,filter_cpp)
