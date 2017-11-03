// Copyright - 2014 - Jan Christoph Uhde <Jan@UhdeJC.com>
// c++11 only
#pragma once
#ifndef OBI_UTIL_SCOPED_TIMER_HPP
#define OBI_UTIL_SCOPED_TIMER_HPP

#include <utility>
#include <vector>
#include <chrono>
#include <functional>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace obi{ namespace util {

class scoped_timer {
public:  // defines
    using high_clock    = std::chrono::high_resolution_clock;
    using clock_string_vec = std::vector<std::pair<high_clock::time_point,std::string>>;
    using int_string_vec = std::vector<std::pair<int,std::string>>;
    using callback_type = std::function<void(int_string_vec const&)>;

private:  // variables
    callback_type callback;
    clock_string_vec timepoints_with_description;
    bool enabled_in_dtor;
    bool add_dtor_entry;

public:  // functions
    scoped_timer(callback_type cb = &scoped_timer::show_internal)
                :callback(cb)
                ,enabled_in_dtor(true)
                ,add_dtor_entry(true)
    {
        timepoints_with_description.reserve(10);  //if you want time more than 10 - add template param?
        timepoints_with_description.emplace_back(high_clock::time_point(),"");
        timepoints_with_description.back().first=high_clock::now(); //defeats the purpose of emplace a bit :/
    }

    scoped_timer(std::string name)
                :callback(&scoped_timer::show_internal)
                ,enabled_in_dtor(true)
                ,add_dtor_entry(true)
    {
        timepoints_with_description.reserve(10);
        timepoints_with_description.emplace_back(high_clock::time_point(), name);
        timepoints_with_description.back().first=high_clock::now();
    }

    ~scoped_timer(void) {
        if(add_dtor_entry) {
            timepoints_with_description.emplace_back(high_clock::now(),"destructor");
        }
        if(enabled_in_dtor) {
            callback(calculate());
        }
    }

    void add_step(std::string const& str = "") {
        timepoints_with_description.emplace_back(high_clock::now(),std::move(str));
    }

    void add_step(std::string&& str = "") {
        timepoints_with_description.emplace_back(high_clock::now(),std::move(str));
    }

    void disable_dtor_entry() {
        add_dtor_entry = false;
    }

    void set_name(std::string name) {
        timepoints_with_description[0].second = name;
    }

private:  // functions
    static long int get_time_diff(high_clock::time_point const& t0, high_clock::time_point const& t1) {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(t1-t0).count();
    }

    int_string_vec calculate() const {
        long int total_time = get_time_diff(timepoints_with_description.front().first
                                           ,timepoints_with_description.back().first
                                           );
        int_string_vec times;
        times.emplace_back(std::make_pair(total_time ,timepoints_with_description.front().second));
        if (timepoints_with_description.size() > 2) {
            for (std::size_t i = 1; i < timepoints_with_description.size(); i++) {
                auto duration = get_time_diff(timepoints_with_description[i-1].first
                                             ,timepoints_with_description[i].first
                                             );
                auto entry = std::make_pair(std::move(duration)
                                           ,timepoints_with_description[i].second
                                           );
                times.emplace_back(std::move(entry));
            }
        }
        return times;
    }  // function - calculate

    static std::stringstream to_string_stream_internal(int_string_vec const& times) {
        std::stringstream ss;
        int width = 15;
        ss << "\ntotal   : "
           << std::setw(width) << times[0].first << " ns - "
           << std::setprecision(8) << std::fixed << times[0].first / 1000.0 << " μs - "
           << std::setprecision(8) << std::fixed << times[0].first / 1000000.0 << " ms - "
           << std::setprecision(8) << std::fixed << times[0].first / 1000000000.0 << " s";

        if (!times[0].second.empty()) {
            ss << " - " << times[0].second;
        }
        ss << std::endl;

        if (times.size() > 1) {
            for (std::size_t i = 1; i < times.size(); i++) {
                ss << "step "  << std::setw(3) << i << ": "
                   << std::setw(width) << times[i].first << " ns"
                   << std::setprecision(1) << std::fixed
                   << " (" << std::setw(5)
                   << 100*static_cast<float>(times[i].first)/static_cast<float>(times[0].first)
                   << "%)";

                if (!times[i].second.empty()) {
                    ss << " - " << times[i].second;
                }
                ss << std::endl;
            }
        }
        return ss;
    }  // function - to_string_stream

    static void show_internal(int_string_vec const& times) {
        std::cerr << to_string_stream_internal(times).rdbuf();
    }
public:
    void run(bool disable = true) {
        if (disable) enabled_in_dtor=false;
        callback(calculate());
    }

    void show(bool disable = true) {
        if (disable) enabled_in_dtor=false;
        show_internal(calculate());
    }

    std::string to_string(bool disable = true) {
        if (disable) enabled_in_dtor=false;
        return to_string_stream_internal(calculate()).str();
    }
} ;

}}  // namespace obi::util
#endif // OBI_UTIL_SCOPED_TIMER_HPP
