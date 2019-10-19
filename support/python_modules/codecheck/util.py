#!/usr/bin/env python3
# Copyright - 2019 - Jan Christoph Uhde <Jan@UhdeJC.com>

import re
from subprocess import Popen, PIPE, STDOUT
from pathlib import Path

def find_clang_format(version = None):
    candidates = [
        "clang-format-ext",
        "clang-format-9",
        "clang-format"
    ]

    for candidate in candidates:
        candidate_path = Path(candidate)
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
            match = version_re.search(out)
            if match:
                return candidate_path

    return None
