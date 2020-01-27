#!/usr/bin/env python
# -*- coding: utf-8 -*-
from pprint import pprint as PP
from conans import ConanFile, CMake, tools, RunEnvironment
import os


class TestPackageConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"

    def build(self):
        cmake = CMake(self)
        cmake.verbose = True
        cmake.configure()
        cmake.build()

    def test(self):
        PP(repr(self.deps_cpp_info.deps))
        PP(repr(self.deps_cpp_info["ext-basics"].rootpath))

        #verify installation
        path = os.path.join(self.deps_cpp_info["ext-basics"].rootpath
                           ,"include", "ext", "macros", "compiler.hpp")
        assert os.path.isfile(path)
        # Validate package import
        with tools.environment_append(RunEnvironment(self).vars):
            bin_path = os.path.join("bin", "test_package")
            self.run(bin_path)
            print(os.path.abspath(bin_path))
