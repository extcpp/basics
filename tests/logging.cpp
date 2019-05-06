#include <gtest/gtest.h>
#include <obi/logging.hpp>
#include <obi/util/except.hpp>
#include <obi/macros/platform.hpp>
#include <cstring>

using namespace std::literals;

class LoggingTest : public ::testing::Test {  };
using LoggingDeathTest = LoggingTest;


TEST_F(LoggingTest, logging_no_crash){
    using namespace obi::logging;
    configuration::threads = false;

    configuration::gdb = true;
    configuration::vim = true;
    configuration::prefix_newline = false;
    configuration::append_newline = true;

    ASSERT_NO_THROW(OBI_LOG("babe") << "cafe?");

    configuration::gdb = false;
    configuration::vim = false;
    configuration::prefix_newline = true;
    configuration::append_newline = false;

    ASSERT_NO_THROW(OBI_LOG("babe") << "2cafe?");
    ASSERT_NO_THROW(OBI_LOG("music", network, warn) << "Green Day");
}

// does not die
#ifndef OBI_COMPILER_VC
TEST_F(LoggingDeathTest, fatal){
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    using namespace obi::logging;
    configuration::threads = false;

    ASSERT_DEATH_IF_SUPPORTED(OBI_LOG("work", network, fatal) << "What?!?! No Cafe!?!?!? :(","");
}
#endif

TEST_F(LoggingTest, threads){
    using namespace obi::logging;
    configuration::threads = true;

    ASSERT_THROW(OBI_LOG("aiaiai") << "No threads ma:(",  obi::util::not_implemented_exception);

    configuration::threads = false;
}

TEST_F(LoggingTest, levels){
    using namespace obi::logging;
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

TEST_F(LoggingTest, levels_to_string){
    using namespace obi::logging;
    configuration::threads = false;

    EXPECT_STREQ(_detail::level_to_str(level::fatal).c_str(), "fatal");
    EXPECT_STREQ(_detail::level_to_str(level::error).c_str(), "error");
    EXPECT_STREQ(_detail::level_to_str(level::warn).c_str(), "warning");
    EXPECT_STREQ(_detail::level_to_str(level::info).c_str(), "info");
    EXPECT_STREQ(_detail::level_to_str(level::debug).c_str(), "debug");
    EXPECT_STREQ(_detail::level_to_str(level::trace).c_str(), "trace");
    EXPECT_STREQ(_detail::level_to_str(static_cast<level>(1337)).c_str(), "unknown");
}
