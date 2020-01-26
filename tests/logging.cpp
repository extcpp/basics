// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include <cstring>
#include <ext/logging.hpp>
#include <ext/macros/platform.hpp>
#include <ext/util/except.hpp>
#include <gtest/gtest.h>

using namespace std::literals;

class LoggingTest : public ::testing::Test {};
using LoggingDeathTest = LoggingTest;

TEST_F(LoggingTest, logging_no_crash) {
    using namespace ext::logging;
    configuration::threads = false;

    configuration::gdb = true;
    configuration::vim = true;
    configuration::prefix_newline = false;
    configuration::append_newline = true;

    ASSERT_NO_THROW(EXT_LOG("babe") << "cafe?");

    configuration::gdb = false;
    configuration::vim = false;
    configuration::prefix_newline = true;
    configuration::append_newline = false;

    ASSERT_NO_THROW(EXT_LOG("babe") << "2cafe?");
    ASSERT_NO_THROW(EXT_LOG("music", network, warn) << "Green Day");
}

// does not die
#ifndef EXT_COMPILER_VC
TEST_F(LoggingDeathTest, fatal) {
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    using namespace ext::logging;
    configuration::threads = false;

    ASSERT_DEATH_IF_SUPPORTED(EXT_LOG("work", network, fatal) << "What?!?! No Cafe!?!?!? :(", "");
}
#endif // EXT_COMPILER_VC

TEST_F(LoggingTest, threads) {
    using namespace ext::logging;
    configuration::threads = true;

    ASSERT_THROW(EXT_LOG("aiaiai") << "No threads ma:(", ext::util::not_implemented_exception);

    configuration::threads = false;
}

TEST_F(LoggingTest, levels) {
    using namespace ext::logging;
    configuration::threads = false;

    EXPECT_TRUE(_detail::level_is_active(level::error));
    EXPECT_TRUE(_detail::level_is_active(level::info));
    EXPECT_FALSE(_detail::level_is_active(level::trace));

    EXPECT_TRUE(_detail::default_level_is_active(level::error));
    EXPECT_TRUE(_detail::default_level_is_active(level::info));
    EXPECT_FALSE(_detail::default_level_is_active(level::trace));

    set_level_all(level::warn);

    EXPECT_TRUE(_detail::level_is_active(level::error));
    EXPECT_FALSE(_detail::level_is_active(level::info));
    EXPECT_FALSE(_detail::level_is_active(level::trace));

    EXPECT_TRUE(_detail::default_level_is_active(level::error));
    EXPECT_TRUE(_detail::default_level_is_active(level::info));
    EXPECT_FALSE(_detail::default_level_is_active(level::trace));
}

TEST_F(LoggingTest, levels_to_string) {
    using namespace ext::logging;
    configuration::threads = false;

    EXPECT_STREQ(_detail::level_to_str(level::fatal).c_str(), "fatal");
    EXPECT_STREQ(_detail::level_to_str(level::error).c_str(), "error");
    EXPECT_STREQ(_detail::level_to_str(level::warn).c_str(), "warning");
    EXPECT_STREQ(_detail::level_to_str(level::info).c_str(), "info");
    EXPECT_STREQ(_detail::level_to_str(level::debug).c_str(), "debug");
    EXPECT_STREQ(_detail::level_to_str(level::trace).c_str(), "trace");
    EXPECT_STREQ(_detail::level_to_str(static_cast<level>(1337)).c_str(), "unknown");
}
