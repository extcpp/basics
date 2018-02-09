#include <gtest/gtest.h>
#include <obi/util/result.hpp>
#include <obi/util/tracing_classes.hpp>

#include <type_traits>

namespace ou = obi::util;

TEST(util_result, lvalue_copy){
    ou::tracing::all all;
    ou::typed_result<ou::tracing::all> result{all};
    bool is_same = std::is_same_v<ou::tracing::all
                                 ,typename ou::typed_result<ou::tracing::all>::value_type
                                 >;
    ASSERT_TRUE(is_same);
}

