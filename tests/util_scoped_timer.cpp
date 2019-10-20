#include <ext/macros/compiler.hpp>
#include <ext/util/scoped_timer.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <thread>

using namespace ext::util;

constexpr auto ms = std::chrono::milliseconds(1);
void assert_time_eq(std::size_t ms_expected, std::pair<std::uint64_t, std::string> const& in, std::size_t steps = 1) {
#ifndef EXT_TESTS_NO_TIME_CRITICAL
    #ifdef EXT_COMPILER_VC
    ms_expected += 2 * steps;
    #else
    ms_expected += steps;
    #endif
    ASSERT_LE(in.first / (1000 * 1000), ms_expected);
#else
    std::cerr << "test not executed\n";
#endif
}

TEST(util_scoped_timer, nostep) {
    scoped_timer_res result;
    auto callback = [&result](scoped_timer_res const& in) {
        result = in;
        return in;
    };

    {
        scoped_timer timer(callback);
        std::this_thread::sleep_for(ms);
    }

    assert_time_eq(1, result.front());
}

TEST(util_scoped_timer, steps) {
    scoped_timer_res result;
    auto callback = [&result](scoped_timer_res const& in) {
        result = in;
        return in;
    };

    {
        scoped_timer timer(callback);
        std::this_thread::sleep_for(ms);
        timer.add_step();
        std::this_thread::sleep_for(ms);
        timer.add_step();
        std::this_thread::sleep_for(ms);
    }

    assert_time_eq(3, result[0], 3);
    assert_time_eq(1, result[1], 1);
    assert_time_eq(1, result[2], 2);
    assert_time_eq(1, result[3], 3);
    ASSERT_STREQ("destructor", result.back().second.c_str());
}

TEST(util_scoped_timer, no_dtor) {
    scoped_timer_res result;
    auto callback = [&result](scoped_timer_res const& in) {
        result = in;
        return in;
    };

    {
        scoped_timer timer(callback);
        timer.disable_dtor_entry();
        std::this_thread::sleep_for(ms);
        timer.add_step();
        std::this_thread::sleep_for(ms);
        timer.add_step();
        std::this_thread::sleep_for(ms);
    }

    assert_time_eq(2, result[0], 2);
    assert_time_eq(1, result[1], 1);
    assert_time_eq(1, result[2], 2);
    ASSERT_STREQ("", result.back().second.c_str());
}

TEST(util_scoped_timer, dismiss) {
    scoped_timer_res result;
    auto callback = [&result](scoped_timer_res const& in) {
        result = in;
        return in;
    };

    {
        scoped_timer timer(callback);
        timer.disable_dtor_entry();
        timer.dismiss();
        timer.init("", 1);
        std::this_thread::sleep_for(ms);
        timer.add_step("fin");

        std::this_thread::sleep_for(ms);
        timer.run();
    }

    assert_time_eq(1, result[0], 1);
    assert_time_eq(1, result[1], 1);
    ASSERT_STREQ("fin", result[1].second.c_str());
}
