#include <gtest/gtest.h>
#include <obi/util/basic.hpp>
#include <string>
using namespace std::literals;

TEST(util_basic, for_each_arg){
    std::vector<int> result;
    auto fun = [&](int a) { result.push_back(a); };
    obi::util::for_each_arg(fun, 1, 2, 3);
    ASSERT_EQ(result, (std::vector<int>{1, 2, 3}));
}

TEST(util_basic, sort_all){
    std::vector<int> a = {2, 1, 4, 3};
    std::vector<int> b = {2, 3, 4, 1};
    auto fun = [&](auto& c) -> void { std::sort(c.begin(),c.end()); };
    obi::util::sort_all(a, b);
    ASSERT_EQ(a, (std::vector<int>{1, 2, 3, 4}));
    ASSERT_EQ(b, (std::vector<int>{1, 2, 3, 4}));
}

TEST(util_basic, filename){
    ASSERT_EQ( ::obi::util::filename("/home/user/foo.bar"), "foo.bar");
}
