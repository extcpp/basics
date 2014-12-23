#include <libobi/util.hpp>
#include <thread>
#include <atomic>
#include <vector>
#include <limits>

volatile bool run;

void fun(int n, int& loc){
    while (!run);

    int current=2;
    std::vector<int> primes(1,2);
    while (current < n)
    {
        current+=1;

        bool isPrime=true;
        for (auto prime : primes)
        {
            if (current % prime == 0)
            {
                isPrime = false;
                break;
            }
        }



        if (isPrime)
        {
            primes.push_back(current);
            loc=current;
        }
    }
}

int main()
{
    run = true;
    //int n = std::numeric_limits<int>::max();
    int n = 100000;
    std::vector<int>numbers(10,1);
    // single thread
    obi::util::scoped_timer timer("start");
    fun(n,numbers[0]);
    fun(n,numbers[1]);
    timer.add_step("single");

    // 2 threads suffering false shring
    run = false;
    auto ta1 = std::thread(fun,n,std::ref(numbers[2]));
    auto ta2 = std::thread(fun,n,std::ref(numbers[3]));
    timer.add_step("threads prepared - false sharing");
    run =  true;
    ta1.join();
    ta2.join();
    timer.add_step("threads - false sharing");

    //2 threads no false sharing
    run = false;
    auto tb1 = std::thread(fun,n,std::ref(numbers[4]));
    auto tb2 = std::thread(fun,n,std::ref(numbers[9]));
    timer.add_step("threads prepared - no false sharing");
    run = true;
    tb1.join();
    tb2.join();
    timer.add_step("threads - no false sharing");

    timer.stop_use_cb();
    for (auto i: numbers)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;

    return 0;
}
