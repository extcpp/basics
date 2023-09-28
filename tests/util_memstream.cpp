#include <gtest/gtest.h>
#include <ext/util/memstream.hpp>
#include <sstream>
#include <string_view>
#include <string>
#include <chrono>

using namespace std::string_literals;

namespace eu = ::ext::util;

TEST(util_memstream, membuff) {
    std::basic_string_view hello_view("hello world");
    auto membuf = eu::make_membuf(hello_view);

    std::basic_string_view goodbye_view("goodbye world");
    membuf.reset(goodbye_view.data(), goodbye_view.length());
}

TEST(util_memstream, imemstream_basic) {
    std::string_view hello_view("hello world");
    auto istream = eu::imemstream<std::string_view::value_type>(hello_view.data(), hello_view.length());
    std::string hello, world, eof;
    istream >> hello;
    ASSERT_EQ(hello, "hello");
    istream >> world;
    ASSERT_EQ(world, "world");
    istream >>  eof;
    ASSERT_EQ(eof, "");
}

TEST(util_memstream, viewstream_size) {
    int size = sizeof(eu::viewstream);
    ASSERT_GE(size, 250);
    ASSERT_LE(size, 450);
}

TEST(util_memstream, viewstream_empty) {
    std::string_view empty_view;
    ASSERT_EQ(empty_view, "");
}

TEST(util_memstream, viewstream_read) {
    std::string_view hello_view("hello world");
    auto istream = eu::viewstream(hello_view);
    std::string hello, world, eof;
    istream >> hello;
    ASSERT_EQ(hello, "hello");
    istream >> world;
    ASSERT_EQ(world, "world");
    istream >>  eof;
    ASSERT_EQ(eof, "");
}

TEST(util_memstream, viewstream_reuse) {
    std::string_view hello_view("hello world");
    std::string_view goodbye_view("goodbye");
    auto istream = eu::viewstream(hello_view);
    istream.reset(goodbye_view);
    std::string goodbye, eof;
    istream >> goodbye;
    ASSERT_EQ(goodbye, "goodbye");
    istream >>  eof;
    ASSERT_EQ(eof, "");
}

TEST(util_memstream, viewstream_getline) {
    std::string_view hello_view("hello world");
    auto istream = eu::viewstream(hello_view);
    std::string line;
    std::getline(istream, line);
    ASSERT_EQ(line, hello_view);
}

TEST(util_memstream, viewstream_chrono) {
    std::string_view a("2023-09-30");
    std::string_view b("2023-11-11");
    auto views = std::vector{a, b};

    eu::viewstream istream;
    //std::chrono::year_month_day ymd;
    std::string date;
    for(auto const& view : views)
    {
        istream.reset(view);
        //std::chrono::from_stream(istream, "%Y-%m-%d", ymd);
        istream >> date;
    }

    //ASSERT_EQ(std::chrono::year_month_day(
    //    std::chrono::year(2023),
    //    std::chrono::month(11),
    //    std::chrono::day(11)), ymd);
    ASSERT_EQ(b, date);
}
