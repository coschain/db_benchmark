//
//  timer.hpp
//  db_benchmarks
//
//  Created by 戴硕 on 2018/10/11.
//

#ifndef timer_hpp
#define timer_hpp

#include <chrono>

template<typename Duration = std::chrono::seconds>
class timer {
public:
    using clock_type = std::chrono::high_resolution_clock;
    using timepoint_type = std::chrono::time_point<clock_type>;
public:
    timer() {
        reset();
    }
    void start() {
        if (!timing) {
            timing = true;
            curr_start = clock_type::now();
        }
    }
    void stop() {
        if (timing) {
            curr_duration += clock_type::now() - curr_start;
            timing = false;
        }
    }
    void reset() {
        timing = false;
        curr_duration = timepoint_type::duration::zero();
    }
    typename Duration::rep duration() {
        return std::chrono::duration_cast<Duration>(timing? curr_duration + (clock_type::now() - curr_start) : curr_duration).count();
    }
private:
    timepoint_type curr_start;
    timepoint_type::duration curr_duration;
    bool timing = false;
};


using ms_timer = timer<std::chrono::milliseconds>;
using us_timer = timer<std::chrono::microseconds>;
using ns_timer = timer<std::chrono::nanoseconds>;

#endif /* timer_hpp */
