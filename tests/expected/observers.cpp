#include "expected.hpp"

struct move_detector {
    move_detector() = default;
    move_detector(move_detector&& rhs) {
        rhs.been_moved = true;
    }
    bool been_moved = false;
};

TEST(expected_observers, observers) {
    eu::expected<int, int> o1 = 42;
    eu::expected<int, int> o2{eu::unexpect, 0};
    const eu::expected<int, int> o3 = 42;

    ASSERT_TRUE(*o1 == 42);
    ASSERT_TRUE(*o1 == o1.value());
    ASSERT_TRUE(o2.value_or(42) == 42);
    ASSERT_TRUE(o2.error() == 0);
    ASSERT_TRUE(o3.value() == 42);
    auto success = std::is_same<decltype(o1.value()), int&>::value;
    ASSERT_TRUE(success);
    success = std::is_same<decltype(o3.value()), const int&>::value;
    ASSERT_TRUE(success);
    success = std::is_same<decltype(std::move(o1).value()), int&&>::value;
    ASSERT_TRUE(success);
    success = std::is_same<decltype(std::move(o3).value()), const int&&>::value;
    ASSERT_TRUE(success);

    eu::expected<move_detector, int> o4{std::in_place};
    move_detector o5 = std::move(o4).value();
    ASSERT_TRUE(o4->been_moved);
    ASSERT_TRUE(!o5.been_moved);
}
