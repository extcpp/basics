// Copyright - 2014 - Jan Christoph Uhde <Jan@UhdeJC.com>
// c++11 only
#pragma once
#ifndef OBI_UTIL_SCOPED_TIMER_HEADER
#define OBI_UTIL_SCOPED_TIMER_HEADER

#include <utility>
#include <vector>
#include <chrono>
#include <functional>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

namespace obi{ namespace util {

template <typename callback_type = void(*)(std::vector<std::pair<std::uint64_t,std::string>> const&)>
class scoped_timer {
public:  // defines
    using high_clock    = std::chrono::high_resolution_clock;
    using clock_string_vec = std::vector<std::pair<high_clock::time_point,std::string>>;
    using int_string_vec = std::vector<std::pair<std::uint64_t,std::string>>;

private:  // variables
    callback_type callback;
    clock_string_vec timepoints_with_description;
    bool enabled_in_dtor;
    bool add_dtor_entry;

public:  // functions
    scoped_timer(callback_type cb = &scoped_timer::show_internal)
        :callback(cb), enabled_in_dtor(true), add_dtor_entry(true) {
        init();
    }

    scoped_timer(std::string const& name)
        :callback(&scoped_timer::show_internal), enabled_in_dtor(true), add_dtor_entry(true) {
        init(name);
    }

    scoped_timer(char const* name)
        :callback(&scoped_timer::show_internal), enabled_in_dtor(true), add_dtor_entry(true) {
        init(name);
    }

    ~scoped_timer(void) {
        if(add_dtor_entry) {
            timepoints_with_description.emplace_back(high_clock::now(), "destructor");
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
    void init(std::string const& name = "") {
        static_assert(std::is_convertible_v<callback_type,std::function<void(int_string_vec const&)>>, "callback-type does not match");
        timepoints_with_description.reserve(10);  //if you want time more than 10 - add template param?
        timepoints_with_description.emplace_back(high_clock::time_point(), name);
        timepoints_with_description.back().first=high_clock::now(); //defeats the purpose of emplace a bit :/
    }

    static std::uint64_t get_time_diff(high_clock::time_point const& t0, high_clock::time_point const& t1) {
        return static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(t1-t0).count());
    }

    int_string_vec calculate() const {
        std::uint64_t total_time = get_time_diff(timepoints_with_description.front().first
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
        int  width = 15;
        ss << "\ntotal   : "
           << std::setw(width) << times[0].first << " ns - "
           << std::setprecision(3) << std::fixed << static_cast<double>(times[0].first) / std::pow(10.0,3.0) << " μs - "
           << std::setprecision(6) << std::fixed << static_cast<double>(times[0].first) / std::pow(10.0,6.0) << " ms - "
           << std::setprecision(9) << std::fixed << static_cast<double>(times[0].first) / std::pow(10.0,9.0) << " s";

        if (!times[0].second.empty()) {
            ss << " - " << times[0].second;
        }
        ss << std::endl;

        if (times.size() > 1) {
            for (std::size_t i = 1; i < times.size(); i++) {
                ss << "step "  << std::setw(3) << i << ": "
                   << std::setw(width) << std::setprecision(6)
                   //<< times[i].first << " ns"
                   << static_cast<double>(times[i].first) / std::pow(10.0,6.0)<< " ms"
                   << std::setprecision(1) << std::fixed
                   << " (" << std::setw(5)
                   << 100*static_cast<double>(times[i].first)/static_cast<double>(times[0].first)
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
    void dismiss() {
        enabled_in_dtor=false;
    }

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
};

}}  // namespace obi::util
#endif // OBI_UTIL_SCOPED_TIMER_HEADER
