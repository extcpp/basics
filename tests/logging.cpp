#include <gtest/gtest.h>
#include <obi/logging.hpp>
#include <cstring>

using namespace std::literals;


TEST(logging, logging_no_crash){
    using namespace obi::logging;
    configuration::gdb = true;
    configuration::vim = true;
    configuration::prefix_newline = false;
    configuration::append_newline = true;

    // TODO add far more tests that check the output
    // This test only asserts that it is somewhat working
    ASSERT_NO_THROW( OBI_LOG("babe") << "cafe?");

    // see: https://github.com/google/googletest/blob/master/googletest/docs/advanced.md#death-tests
    //ASSERT_DEATH(OBI_LOG("cafe", fatal) << "No cafe?!?!?!!", ".*No cafe.*");
}

TEST(logging, levels){
    using namespace obi::logging;
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

TEST(logging, levels_to_string){
    using namespace obi::logging;

    EXPECT_STREQ(_detail::level_to_str(level::fatal).c_str(), "fatal");
    EXPECT_STREQ(_detail::level_to_str(level::error).c_str(), "error");
    EXPECT_STREQ(_detail::level_to_str(level::warn).c_str(), "warning");
    EXPECT_STREQ(_detail::level_to_str(level::info).c_str(), "info");
    EXPECT_STREQ(_detail::level_to_str(level::debug).c_str(), "debug");
    EXPECT_STREQ(_detail::level_to_str(level::trace).c_str(), "trace");
    EXPECT_STREQ(_detail::level_to_str(static_cast<level>(1337)).c_str(), "unknown");
}
