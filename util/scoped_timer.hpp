// Copyright - 2014 - Jan Christoph Uhde <Jan@UhdeJC.com>
// c++11 only

#include <utility>
#include <vector>
#include <chrono>
#include <functional>
#include <iostream>
#include <iomanip>

namespace obi{ namespace util{

class scoped_timer
{
public:  // defines
    using hclock    = std::chrono::high_resolution_clock;
    using hrcStrVec = std::vector<std::pair<hclock::time_point,std::string>>;
    using intStrVec = std::vector<std::pair<int,std::string>>;

public:  // functions
    scoped_timer(std::function<intStrVec(intStrVec)> callback = &scoped_timer::toSting):
        m_callback(callback),
        m_disabled(false),
        m_disableAdd(false)
    {
        m_timePoints.reserve(10);  //if you want time more than 10 - add template param?
        m_timePoints.emplace_back(hclock::time_point(),"");
        m_timePoints.back().first=hclock::now(); //defeats the purpose of emplace a bit :/
    }

    scoped_timer(std::string name):
        m_callback(&scoped_timer::toSting),
        m_disabled(false),
        m_disableAdd(false)
    {
        m_timePoints.reserve(10);
        m_timePoints.emplace_back(hclock::time_point(), name);
        m_timePoints.back().first=hclock::now();
    }

    ~scoped_timer(void)
    {
        m_timePoints.emplace_back(hclock::now(),"dtor");
        if(m_disableAdd)
        {
           m_timePoints.pop_back();
        }
        if(!m_disabled)
        {
            m_callback(calculate());
        }
    }

    void add_step(std::string&& str = "")
    {
        m_timePoints.emplace_back(hclock::now(),str);
    }

    void disable_dtor_add()
    {
        m_disableAdd = true;
    }

    intStrVec stop_use_cb()
    {
        m_disabled=true;
        return m_callback(calculate());
    }

    void set_name(std::string name)
    {
        m_timePoints[0].second = name;
    }

private:  // variables
    hrcStrVec m_timePoints;
    std::function<intStrVec(intStrVec)> m_callback;
    bool m_disabled;
    bool m_disableAdd;

private:  // functions
    static int get_time_diff(const hclock::time_point& t0, const hclock::time_point& t1)
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(t1-t0).count();
    }

    intStrVec calculate()
    {
        int total_time = get_time_diff(
            m_timePoints.front().first,
            m_timePoints.back().first
        );
        intStrVec times;
        times.emplace_back(
                std::make_pair(total_time,m_timePoints.front().second)
        );
        if (m_timePoints.size() > 2)
        {
            for (int i = 1; i < m_timePoints.size(); i++)
            {
                times.emplace_back(
                        std::make_pair(
                            get_time_diff(m_timePoints[i-1].first,m_timePoints[i].first),
                            m_timePoints[i].second
                        )
                );
            }
        }
        return std::move(times);
    }  // function - calculate

    static intStrVec toSting(intStrVec times)
    {
        int width = 15;
        std::cout << "\ntotal   : "
                  << std::setw(width) << times[0].first << " ns";

        if (!times[0].second.empty())
        {
            std::cout << " - " << times[0].second;
        }
        std::cout << std::endl;

        if (times.size() > 1)
        {
            for (int i = 1; i < times.size(); i++)
            {
                std::cout << "step "  << std::setw(3) << i << ": "
                          << std::setw(width) << times[i].first << " ns"
                          << std::setprecision(1) << std::fixed
                          << " (" << 100*static_cast<float>(times[i].first)/times[0].first << "%)";
                if (!times[i].second.empty())
                {
                    std::cout << " - " << times[i].second;
                }
                std::cout << std::endl;
            }
        }
        return std::move(times);
    }  // function - to_string
};

}}
