#!/usr/bin/env python3
# Copyright - 2018 - Jan Christoph Uhde <Jan@UhdeJC.com>

from typing import Dict, Tuple, List
import types
import os
import sys
import json

try:
    import git
except ImportError as e:
    print("please install the 'git' module (e.g `apt-get install python3-git`")
    sys.exit(1)
except ModuleNotFoundError as e:
    print("please install the 'git' module (e.g `apt-get install python3-git`")
    sys.exit(1)

# path helper
import inspect
from inspect import currentframe, getframeinfo
from pathlib import Path

def get_current_file() -> Path:
    #filename = inspect.getframeinfo(currentframe()).filename
    filename = (inspect.stack()[0]).filename
    return Path(filename).resolve()

def get_current_path() -> Path:
    return get_current_file().parent
# path helper - end

# git
class GitCheckoutProgress:
    """progress hander for git module"""
    def __init__(self, fh):
        self.progress = 0
        self.fh = fh
        self.sep = ""
    def __call__(self, *args):
        loaded = args[1]
        total = args[2]
        new_progress = round(( loaded / total ) * 100,0)
        if new_progress - self.progress > 9.5:
            #out = "{0:3.0f}% -".format(self.progress)
            out = "{0}{1:.0f}% -".format(self.sep, self.progress)
            self.fh.write(out)
            self.fh.flush()
            self.sep = " "
            self.progress = new_progress

def git_checkout_repo(location: str, branch: str, directory: Path):
    """checks out git repository"""
    print("cloning reposotry: {0} to\n{2}".format(location, branch, directory), flush=True)
    repo = git.Repo.clone_from(location, str(directory), GitCheckoutProgress(sys.stdout))
    print(" done", flush = True)
    print("checking out {}".format(branch), end = "")
    checkout_result = repo.git.checkout(branch)
    print(" - done", flush = True)
    ##print(checkout_result) -- TODO find out how to check the result
# git - end

def init_libs(json_file: str, json_dir: Path, checkout_dir: Path, path_filter = None) -> int:
    """initializes libs described by json_file"""

    json_file = json_dir.joinpath(json_file)
    parsed = None ## will contain the parsed json
    with open(json_file) as fh:
        parsed = json.load(fh)

    repos = parsed["repositories"]
    if not repos or type(repos) is not list:
        raise ValueError("json does not contain repostories list")

    for repo in repos:
        location = repo["location"]
        if not location or type(location) is not str:
            raise ValueError("repository does not have valid source a location")

        branch = repo["branch"]
        if not branch:
            branch = "master"
        elif branch and type(branch) is not str:
            raise ValueError("repository does not have a valid branchname or tag")

        directory = repo["directory"]
        if not directory:
            directory = location.split("/")[-1]

        if not directory or type(directory) is not str:
            raise ValueError("repository does not have a valid target directory")

        target_dir = checkout_dir.joinpath(directory)

        if ( isinstance(path_filter, types.FunctionType) and path_filter(target_dir) ) or not path_filter:
            result = git_checkout_repo(location, branch, target_dir)
            if result:
                return result
        else:
            print("skipping repository {}".format(location))

    return 0

if __name__ == "__main__":
    current_dir = get_current_path()
    json_file = None
    if len(sys.argv) < 2:
        json_file = current_dir.joinpath("libs.json")
    else:
        json_file = current_dir.joinpath(sys.argv[1])


    path_filter = lambda x : not x.exists()

    result = init_libs(json_file, json_dir = current_dir, checkout_dir = current_dir, path_filter = path_filter)
    sys.exit(result)
