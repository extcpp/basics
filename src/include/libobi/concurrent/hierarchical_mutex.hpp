// The code is taken from: "Anthony Williams: C++ Concurrency in Action"
// c++11 only
#pragma once

#include <mutex>
#include <stdexcept>
#include <limits>

namespace obi{ namespace concurrent{

class hierarchical_mutex
{
public: // functions
    explicit hierarchical_mutex(unsigned long value):
        hierarchy_value(value),
        previous_hierarchy_value(0)
    {}
    void lock()
    {
        check_for_hierarchy_violation();
        internal_mutex.lock();
        update_hierarchy_value();
    }
    void unlock()
    {
        this_thread_hierarchy_value=previous_hierarchy_value;
        internal_mutex.unlock();
    }
    bool try_lock()
    {
        check_for_hierarchy_violation();
        if(!internal_mutex.try_lock())
        {
            return false;
        }
        update_hierarchy_value();
        return true;
    }

private:  // variables
    std::mutex internal_mutex;
    unsigned long const hierarchy_value;
    unsigned long previous_hierarchy_value;
    static thread_local unsigned long this_thread_hierarchy_value;

private:  // functions
    void check_for_hierarchy_violation()
    {
        if(this_thread_hierarchy_value <= hierarchy_value)
        {
            throw std::logic_error("mutex hierarchy violated");
        }
    }
    void update_hierarchy_value()
    {
        previous_hierarchy_value=this_thread_hierarchy_value;
        this_thread_hierarchy_value=hierarchy_value;
    }
};

thread_local unsigned long
hierarchical_mutex::this_thread_hierarchy_value(std::numeric_limits<unsigned long>::max());

}}  // namespace obi::thread
