#include <gtest/gtest.h>

#include <obi/meta/has_member.hpp>
#include <iostream>

using namespace std;
using namespace obi::meta;

struct leet {
        using type = void;
        static const int value = 1337;
        using category = string;
};

struct unleet {};

TEST(meta_has_member, has_var_value){
    ASSERT_TRUE(has_var_value_v<leet>);
    ASSERT_FALSE(has_var_value_v<unleet>);
}

TEST(meta_has_member, has_type_type){
    ASSERT_TRUE(has_type_type_v<leet>);
    ASSERT_FALSE(has_type_type_v<unleet>);
}

TEST(meta_has_member, has_type_category){
    ASSERT_TRUE(has_type_category_v<leet>);
    ASSERT_FALSE(has_type_category_v<unleet>);
}

TEST(meta_has_member, is_input_iterator){
    ASSERT_TRUE(is_input_iterator_v<std::istreambuf_iterator<char>>);
    ASSERT_FALSE(is_input_iterator_v<std::ostreambuf_iterator<char>>);
}
