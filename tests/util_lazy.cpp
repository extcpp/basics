#include <gtest/gtest.h>

#include <ext/util/lazy.hpp>
#include <unordered_map>

TEST(util_lazy, lazy_construct) {
    std::unordered_map<int, int> map;

    {
        auto [it, emplaced] = map.try_emplace(1, ext::util::lazy_construct([] {
                                                  return 1;
                                              }));
        ASSERT_TRUE(emplaced);
        ASSERT_EQ(map[1], 1);
    }


    {
        bool lambda_not_used = true;
        auto [it, emplaced] = map.try_emplace(1, ext::util::lazy_construct([&] {
                                                  lambda_not_used = false;
                                                  return 42;
                                              }));
        ASSERT_FALSE(emplaced);
        ASSERT_EQ(map[1], 1);
        ASSERT_TRUE(lambda_not_used);
    }
}
