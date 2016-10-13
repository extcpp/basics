#include <obi/util/scoped_timer.hpp>
#include <thread>
#include <atomic>
#include <vector>
#include <iostream>

volatile bool run;

void fun(int n, int& loc){
    while (!run);
    int current=2;
    std::vector<int> primes(1,2);
    while (current < n){
        current+=1;
        bool isPrime=true;
        for (auto prime : primes){
            if (current % prime == 0){
                isPrime = false;
                break;
            }
        }
        if (isPrime){
            primes.push_back(current);
            loc=current;
        }
    }
}

int main(){
    obi::util::scoped_timer timer("overall calulation time");
    run = true;
    int n = 100000;
    size_t vec_len = 10;
    std::vector<int>numbers(vec_len,0);

    // single thread
    timer.add_step("single thread - prepared");
    fun(n,numbers[0]);
    fun(n,numbers[1]);
    timer.add_step("single thread - done");

    // 2 threads suffering false shring
    run = false;
    auto ta1 = std::thread(fun,n,std::ref(numbers[2]));
    auto ta2 = std::thread(fun,n,std::ref(numbers[3]));
    timer.add_step("threads (false sharing) - prepared ");
    run = true;
    ta1.join();
    ta2.join();
    timer.add_step("threads (false sharing) - done ");

    //2 threads no false sharing
    run = false;
    auto tb1 = std::thread(fun,n,std::ref(numbers[4]));
    auto tb2 = std::thread(fun,n,std::ref(numbers[9]));
    timer.add_step("threads - prepard");
    run = true;
    tb1.join();
    tb2.join();
    timer.add_step("threads - done");

    //print result
    timer.disable_dtor_entry();
    std::cout << std::endl;
    for (size_t i = 0; i < numbers.size(); i++ )
    {
        std::cout << i << ": " << numbers[i] << std::endl;
    }
    std::cout << std::endl;
    return 0;
}
