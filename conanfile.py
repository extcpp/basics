#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from conans import ConanFile, CMake, tools

class ExtbaseConan(ConanFile):
    name = "ext-basics"
    version = "0.0.1"
    license = "MIT"
    author = "Jan Christoph Uhde <jan@uhdejc.com>"
    url = "https://github.com/extcpp/libext"
    description = "Extended C++ - base library"
    topics = ("c++", "utils", "library")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": True}
    generators = "cmake"

    def source(self):
        self.run("git clone -b conan https://github.com/extcpp/libext.git basics")

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="basics")
        cmake.build()

        # Explicit way:
        # self.run('cmake %s/hello %s'
        #          % (self.source_folder, cmake.command_line))
        # self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        """install / copy artifarct"""
        cmake = CMake(self)
        cmake.definitions["LIBEXT_TESTS"] = False
        cmake.definitions["LIBEXT_WARNINGS"] = False
        cmake.definitions["LIBEXT_EXAMPLES"] = False
        cmake.configure()
        #cmake.build()

        self.copy("*.h", dst="include", src="src")
        self.copy("*hello.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["libext-basics-shared.so"]

