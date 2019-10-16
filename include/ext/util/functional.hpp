// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>
#ifndef EXT_UTIL_FUNCTIONAL_HEADER
#define EXT_UTIL_FUNCTIONAL_HEADER

namespace ext { namespace util {

// basic
auto add = [](auto x, auto y) {
    return x + y;
};

auto sub = [](auto x, auto y) {
    return x - y;
};

auto mul = [](int x, int y) {
    return x * y;
};

// higher order
auto apply = [](auto function) {
    return [=](auto... tail) {
        return function(tail...);
    };
};

auto apply_unary = [](auto binary) {
    return [=](auto x) {
        return binary(x, x);
    };
};

auto apply_binary = [](auto binary) {
    return [=](auto x) {
        return [=](auto y) {
            return binary(x, y);
        };
    };
};

auto curry = [](auto function, auto head) {
    return [=](auto... tail) {
        return function(head, tail...);
    };
};

// lists
auto list = [](auto... xs) {
    return [=](auto access) {
        return access(xs...);
    };
};

auto head = [](auto xs) {
    return xs([](auto y, auto... /*ys*/) {
        return y;
    });
};

auto tail = [](auto xs) {
    return xs([](auto /*y*/, auto... ys) {
        return list(ys...);
    });
};

auto length = [](auto xs) {
    return xs([](auto... yys) {
        return sizeof...(yys);
    });
};
}}     // namespace ext::util
#endif // EXT_UTIL_FUNCTIONAL_HEADER
