#pragma once

#include <ostream>
#include <cmath>

using Time = float;

constexpr Time INVALID_TIME = NAN;

struct Time_Interval {
public:
    Time start = INVALID_TIME;
    Time end = INVALID_TIME;

    Time_Interval() {}
    Time_Interval(Time start, Time end) : start(start), end(end) {}
    Time delta() const;
};

std::ostream& operator<<(std::ostream& os, Time_Interval const& interval);
