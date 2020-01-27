// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include <gtest/gtest.h>

// The `version.hpp` should be written to the
// build dir of the executable to avoid the
// relative search. But this is still better
// than polluting the source directory.
#if __has_include(<../ ext_version.hpp>)
    #include <../ext_version.hpp>
#else
constexpr char const* GIT_REV = "unknown";
constexpr char const* GIT_BRANCH = "unknown";
#endif // __has_include(<../ ext_version.hpp>)

int main(int argc, char** argv) {
    std::cout << "git revision: " << GIT_REV << std::endl;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
