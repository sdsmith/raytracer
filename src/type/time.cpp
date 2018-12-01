#include "time.h"

Time Time_Interval::delta() const { return end - start; }

std::ostream& operator<<(std::ostream& os, Time_Interval const& interval)
{
    return os << "[" << interval.start << "," << interval.end << "]";
}
