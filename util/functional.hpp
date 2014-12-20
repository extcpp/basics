#pragma once

namespace obi { namespace functional {

auto mul = [](int x, int y) {
    return x * y;
};

auto add = [](auto x, auto y) {
    return x + y;
};

auto sub = [](auto x, auto y) {
    return x - y;
};

auto apply_unary =[](auto binary) {
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

auto list = [](auto ...xs) {
    return [=](auto access) { return access(xs...); };
};

auto head = [](auto xs) {
    return xs([](auto first, auto ...rest) { return first; });
};

auto tail = [](auto xs) {
    return xs([](auto first, auto ...rest) { return list(rest...); });
};

auto length = [](auto xs) {
    return xs([](auto ...z) { return sizeof...(z); });
};

}}  // obi::functional
