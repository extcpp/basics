// Copyright - 2020 - Jan Christoph Uhde <Jan@UhdeJC.com>
// Please see LICENSE.md for license or visit https://github.com/extcpp/basics
#include <ext/util/expected.hpp>
#include <stdexcept>
#include <string>
namespace eu = ext::util;

struct no_throw {
    no_throw(std::string s) : i(s) {}
    std::string i;
};

struct canthrow_move {
    canthrow_move(std::string s) : i(s) {}
    canthrow_move(canthrow_move const&) = default;
    canthrow_move(canthrow_move&& other) noexcept(false) : i(other.i) {}
    canthrow_move& operator=(canthrow_move&&) = default;
    std::string i;
};

bool should_throw = false;

struct willthrow_move {
    willthrow_move(std::string s) : i(s) {}
    willthrow_move(willthrow_move const&) = default;
    willthrow_move(willthrow_move&& other) : i(other.i) {
        if (should_throw) {
            throw std::runtime_error("throw on purpose");
        }
    }
    willthrow_move& operator=(willthrow_move&&) = default;
    std::string i;
};

int main() {
    std::string s1 = "abcdefghijklmnopqrstuvwxyz";
    std::string s2 = "zyxwvutsrqponmlkjihgfedcbaxxx";

    eu::expected<no_throw, willthrow_move> a{s1};
    eu::expected<no_throw, willthrow_move> b{eu::unexpect, s2};

    should_throw = true;
    bool has_thrown = false;

    try {
        swap(a, b);
    } catch (std::exception const& ex) {
        has_thrown = true;
    }

    if (should_throw != has_thrown) {
        return 1;
    } else {
        return 0;
    }
}
