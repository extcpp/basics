#include <gtest/gtest.h>
#include <obi/util/result.hpp>
#include <obi/util/tracing_classes.hpp>

#include <type_traits>

namespace ou = obi::util;
using namespace obi::util::tracing;

TEST(util_result, lvalue_copy){
    all value;
    ou::typed_result<all> result{value};

    ASSERT_EQ(result.value.default_ctor, 1);
    ASSERT_EQ(result.value.copy_ctor, 1);
    ASSERT_EQ(result.value.opearations(), 2);

    ASSERT_TRUE((std::is_same_v<decltype(value), decltype(result.value)>));
    ASSERT_TRUE((std::is_same_v<all, typename ou::typed_result<all>::value_type>));
}

TEST(util_result, rvalue){
    ou::typed_result<all> result(all{});

    ASSERT_EQ(result.value.default_ctor, 1);
    ASSERT_EQ(result.value.move_ctor, 1);
    ASSERT_EQ(result.value.opearations(), 2);
}

TEST(util_result, rvalue_no_move_ctor){
    ou::typed_result<no_move_ctor> result(no_move_ctor{});

    ASSERT_EQ(result.value.default_ctor, 1);
    ASSERT_EQ(result.value.move_assign, 1);
    ASSERT_EQ(result.value.opearations(), 2);
}

TEST(util_result, rvalue_no_move_assign){
    ou::typed_result<no_move_assign> result(no_move_assign{});

    ASSERT_EQ(result.value.default_ctor, 1);
    ASSERT_EQ(result.value.move_ctor, 1);
    ASSERT_EQ(result.value.opearations(), 2);
}

TEST(util_result, lvalue_reference){
    all value;
    ou::typed_result<all&> result{value};

    ASSERT_EQ(result.value.default_ctor, 1);
    ASSERT_EQ(result.value.opearations(), 1);
}

TEST(util_result, pointer){
    all value;
    ou::typed_result<all*> result(&value);

    ASSERT_EQ(result.value->default_ctor, 1);
    ASSERT_EQ(result.value->opearations(), 1);
    ASSERT_TRUE((std::is_same_v<decltype(&value), decltype(result.value)>));
}
