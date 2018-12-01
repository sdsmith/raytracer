#pragma once

#include <ostream>
#include <cmath>

using Time = float;

constexpr Time INVALID_TIME = NAN;

struct Time_Interval {
public:
    Time start;
    Time end;

    Time delta() const;
};

std::ostream& operator<<(std::ostream& os, Time_Interval const& interval);
