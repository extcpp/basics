// Copyright - 2014 - Jan Christoph Uhde <Jan@UhdeJC.com>
// c++11 only

#include <utility>
#include <vector>
#include <chrono>
#include <functional>
#include <iostream>
#include <iomanip>

namespace o{ namespace util{

class ScopedTimer
{
    using hc = std::chrono::high_resolution_clock;
    using hrcStrVec = std::vector<std::pair<hc::time_point,std::string>>;
    using intStrVec = std::vector<std::pair<int,std::string>>;

    hrcStrVec m_timePoints;
    std::function<void(intStrVec)> m_callback;

    bool m_disabled;
    bool m_disableAdd;

    int getTimeDiff(hc::time_point& t0, hc::time_point& t1)
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(t1-t0).count();
    }

    intStrVec calculate()
    {
        int total_time = getTimeDiff(
            m_timePoints.front().first,
            m_timePoints.back().first
        );
        intStrVec times;
        times.push_back(
                std::make_pair(total_time,m_timePoints.front().second)
        );
        if (m_timePoints.size() > 2)
        {
            for (int i = 1; i < m_timePoints.size(); i++)
            {
                times.push_back(
                        std::make_pair(
                            getTimeDiff(m_timePoints[i-1].first,m_timePoints[i].first),
                            m_timePoints[i].second
                        )
                );
            }
        }
        return std::move(times);
    }

    static void toSting(intStrVec times)
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
                          << std::setw(width) << times[i].first << " ns";
                if (!times[i].second.empty())
                {
                    std::cout << " - " << times[i].second;
                }
                std::cout << std::endl;
            }
        }
    }

    void stopUseCB()
    {
        m_disabled=true;
        m_callback(calculate());
    }

public:

    ScopedTimer(std::function<void(intStrVec)> callback = &ScopedTimer::toSting) :
        m_timePoints{std::make_pair(hc::now(),std::string())},
        m_callback(callback),
        m_disabled(false),
        m_disableAdd(false)
    {}

    ScopedTimer(std::string name) :
        m_timePoints{std::make_pair(hc::now(),name)},
        m_callback(&ScopedTimer::toSting),
        m_disabled(false),
        m_disableAdd(false)
    {}

    ~ScopedTimer(void)
    {
        m_timePoints.emplace_back(hc::now(),"dtor");
        if(m_disableAdd)
        {
           m_timePoints.pop_back();
        }
        if(!m_disabled)
        {
            m_callback(calculate());
        }
    }

    void addStep(std::string&& str = "")
    {
        m_timePoints.emplace_back(std::make_pair(hc::now(),str));
    }

    void disableDtorAdd()
    {
        m_disableAdd = true;
    }

    void setName(std::string name)
    {
        m_timePoints[0].second = name;
    }
};

}} //namespace o::util
