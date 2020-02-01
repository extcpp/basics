// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include <algorithm>
#include <gtest/gtest.h>

#include <ext/math/geometry/simple_shapes.hpp>

TEST(math_simple_shapes, construct) {
    using namespace ext::math;

    {
        auto v1 = vec{2,2,2};
        auto v2 = vec{3,3,3};
        ASSERT_NO_THROW(geometry::rectangle(v1, v2));
    }

    {
        auto v1 = vec{2,4,2};
        auto v2 = vec{3,1,3};
        auto rect = geometry::rectangle(v1, v2);
        ASSERT_FALSE(rect.validate());
    }

    {
        auto v1 = vec{2,4,2};
        auto v2 = vec{3,1,3};
        ASSERT_THROW((geometry::rectangle<int, 3, true>(v1, v2)), std::invalid_argument);
    }
}

TEST(math_simple_shapes, size) {
    using namespace ext::math;

    auto v1 = vec{2,2,2};
    auto v2 = vec{3,3,3};

    auto rect = geometry::rectangle(v1, v2);

    ASSERT_EQ(rect.size(), (vec{1,1,1}));
    ASSERT_NE(rect.size(), (vec{1,2,3}));
}
