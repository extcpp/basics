// Copyright - 2016 - Jan Christoph Uhde <Jan@UhdeJC.com>
#include <array>
#include <iostream>
#include <vector>

#include <ext/structures/binary_index_tree.hpp>

int main(/*int argc, const char *argv[]*/) {
    std::size_t inc = 1;
    std::size_t dec = 31;
    for (std::size_t i = 0; i < 6; i++) {
        std::cout << i << ": " << inc << " " << dec << std::endl;
        ext::structures::detail::increase_lsb(inc);
        ext::structures::detail::remove_lsb(dec);
    }

    std::array<long, 16> arr{};

    auto begin = std::begin(arr);
    auto end = std::end(arr);

    std::array<unsigned, 16> input = {{0, 2, 0, 1, 1, 1, 0, 4, 4, 0, 1, 0, 1, 2, 3, 0}};

    for (auto it = input.begin(); it != input.end(); it++) {
        ext::structures::bit_modify(begin, end, static_cast<std::size_t>(std::distance(input.begin(), it)), *it);
    }

    // index
    std::cout << "idx:      ";
    for (std::size_t j = 0; j < arr.size(); ++j) {
        std::cout << j << "\t";
    }
    std::cout << std::endl;

    // input
    std::cout << "input:    ";
    for (auto& i : input) {
        std::cout << i << "\t";
    }
    std::cout << std::endl;

    // get
    std::cout << "get:      ";
    for (std::size_t j = 0; j < arr.size(); ++j) {
        std::cout << ext::structures::bit_get<long>(begin, end, j) << "\t";
    }
    std::cout << std::endl;

    // cul
    std::cout << "cul:      ";
    for (std::size_t j = 0; j < arr.size(); ++j) {
        std::cout << ext::structures::bit_get_sum<long>(begin, end, j) << "\t";
    }
    std::cout << std::endl;

    // stored
    std::cout << "store:    ";
    for (auto& i : arr) {
        std::cout << i << "\t";
    }
    std::cout << std::endl;

    ext::structures::bit_set(begin, end, std::size_t(13), long(1337));
    std::cout << ext::structures::bit_get<long>(begin, end, std::size_t(13)) << std::endl;

    return 0;
}
