#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from conans import ConanFile, CMake, tools

class ExtbaseConan(ConanFile):
    name = "ext-basics"
    version = "0.0.1"
    license = "MIT"
    author = "Jan Christoph Uhde <jan@uhdejc.com>"
    url = "https://github.com/extcpp/basics"
    description = "Extended C++ - base library"
    topics = ("c++", "utils", "library")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": True}
    generators = "cmake"

    def source(self):
        self.run("git clone -b conan https://github.com/extcpp/basics.git basics --recurse-submodules")

    def build(self):
        cmake = CMake(self)
        #cmake.verbose = True
        cmake.configure(source_folder="basics")
        cmake.build()

    def package(self):
        """install / copy artifarct"""
        cmake = CMake(self)
        #cmake.verbose = True
        cmake.definitions["LIBEXT_TESTS"] = False
        cmake.definitions["LIBEXT_WARNINGS"] = False
        cmake.definitions["LIBEXT_EXAMPLES"] = False
        cmake.configure()
        cmake.install()

    def package_info(self):
        self.info.header_only()

