// Copyright - xxxx-2019 - Jan Christoph Uhde <Jan@UhdeJC.com>
#include <atomic>
#include <ext/util/scoped_timer.hpp>
#include <iostream>
#include <thread>
#include <vector>

volatile bool run;

void fun(std::uint32_t n, std::uint32_t& loc) {
    while (!run) {
    };
    std::uint32_t current = 2;
    std::vector<uint32_t> primes(1, 2);
    while (current < n) {
        current += 1;
        bool isPrime = true;
        for (auto const& prime : primes) {
            if (current % prime == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime) {
            primes.push_back(current);
            loc = current;
        }
    }
}

int main() {
    { ext::util::scoped_timer timer("no steps"); }
    { ext::util::scoped_timer timer("no steps"); }
    {
        ext::util::scoped_timer timer("one step - no dtor");
        timer.add_step();
        timer.disable_dtor_entry();
    }
    {
        ext::util::scoped_timer timer("one step");
        timer.add_step();
    }

    std::size_t vec_len = 10;
    std::vector<std::uint32_t> numbers(vec_len, 0);

    {
        ext::util::scoped_timer timer("overall calulation time");
        run = true;
        std::uint32_t n = 100000;

        // single thread
        timer.add_step("single thread - prepared");
        fun(n, numbers[0]);
        fun(n, numbers[1]);
        timer.add_step("single thread - done");

        // 2 threads suffering false shring
        run = false;
        auto ta1 = std::thread(fun, n, std::ref(numbers[2]));
        auto ta2 = std::thread(fun, n, std::ref(numbers[3]));
        timer.add_step("threads (false sharing) - prepared ");
        run = true;
        ta1.join();
        ta2.join();
        timer.add_step("threads (false sharing) - done ");

        // 2 threads no false sharing
        run = false;
        auto tb1 = std::thread(fun, n, std::ref(numbers[4]));
        auto tb2 = std::thread(fun, n, std::ref(numbers[9]));
        timer.add_step("threads - prepard");
        run = true;
        tb1.join();
        tb2.join();
        timer.add_step("threads - done");

        // print result
        timer.disable_dtor_entry();
    }

    std::cout << std::endl;
    for (std::size_t i = 0; i < numbers.size(); i++) {
        std::cout << i << ": " << numbers[i] << std::endl;
    }
    std::cout << std::endl;

    return 0;
}
