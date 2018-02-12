#include <gtest/gtest.h>
#include <obi/util/result.hpp>
#include <obi/util/tracing_classes.hpp>

#include <type_traits>

namespace ou = obi::util;
using namespace obi::util::tracing;

TEST(util_result, lvalue_copy){
    all value;
    ou::typed_result<all> result{value};
    ASSERT_TRUE(result.value.copy_ctor);
    ASSERT_TRUE((std::is_same_v<all, typename ou::typed_result<all>::value_type>));
    ASSERT_TRUE((std::is_same_v<decltype(value), decltype(result.value)>));
}

TEST(util_result, rvalue){
    ou::typed_result<all> result(all{});
    ASSERT_TRUE(result.value.move_ctor);
    ASSERT_TRUE((std::is_same_v<all, typename ou::typed_result<all>::value_type>));
    result.value.print();
}
