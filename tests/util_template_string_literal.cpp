// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include <gtest/gtest.h>
#include <ext/util/template_string_literal.hpp>

using namespace ext::util;

template<template_string_literal lit>
struct example_class {
    const std::string name  = lit.value;
};

TEST(util_template_string_literal, example) {
    example_class<"foo"> actual;
    ASSERT_EQ(actual.name, "foo");
}
