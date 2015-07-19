#include "libobi/concurrent/threadsafe_queue.hpp"
#include "libobi/concurrent/threadsafe_stack.hpp"

int main(){
    obi::concurrent::threadsafe_queue<int> queue;
    obi::concurrent::threadsafe_stack<int> stack;
    return 0;
}
