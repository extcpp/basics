#include <ext/util/result.hpp>
#include <ext/util/tracing_classes.hpp>
#include <gtest/gtest.h>

#include <type_traits>

namespace eu = ext::util;
using namespace ext::util::tracing;

TEST(util_result, lvalue_copy) {
    all value;
    eu::typed_result<all> result{value};

    ASSERT_EQ(result.value.default_ctor, 1);
    ASSERT_EQ(result.value.copy_ctor, 1);
    ASSERT_EQ(result.value.opearations(), 2);

    ASSERT_TRUE((std::is_same_v<decltype(value), decltype(result.value)>) );
    ASSERT_TRUE((std::is_same_v<all, typename eu::typed_result<all>::value_type>) );
}

TEST(util_result, rvalue) {
    eu::typed_result<all> result(all{});

    ASSERT_EQ(result.value.default_ctor, 1);
    ASSERT_EQ(result.value.move_ctor, 1);
    ASSERT_EQ(result.value.opearations(), 2);
}

TEST(util_result, rvalue_no_move_ctor) {
    eu::typed_result<no_move_ctor> result(no_move_ctor{});

    ASSERT_EQ(result.value.default_ctor, 1);
    ASSERT_EQ(result.value.move_assign, 1);
    ASSERT_EQ(result.value.opearations(), 2);
}

TEST(util_result, rvalue_no_move_assign) {
    eu::typed_result<no_move_assign> result(no_move_assign{});

    ASSERT_EQ(result.value.default_ctor, 1);
    ASSERT_EQ(result.value.move_ctor, 1);
    ASSERT_EQ(result.value.opearations(), 2);
}

TEST(util_result, lvalue_reference) {
    all value;
    eu::typed_result<all&> result{value};

    ASSERT_EQ(result.value.default_ctor, 1);
    ASSERT_EQ(result.value.opearations(), 1);
}

TEST(util_result, pointer) {
    all value;
    eu::typed_result<all*> result(&value);

    ASSERT_EQ(result.value->default_ctor, 1);
    ASSERT_EQ(result.value->opearations(), 1);
    ASSERT_TRUE((std::is_same_v<decltype(&value), decltype(result.value)>) );
}
