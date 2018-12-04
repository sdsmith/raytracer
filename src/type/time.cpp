#include "time.h"

//------------------------------------------------------------------------------
// Time_Delta

float operator/(Time_Delta const& lhs, Time_Delta const& rhs) {
    return static_cast<float>(lhs) / static_cast<float>(rhs);
}

Time_Delta operator*(Time_Delta const& lhs, float rhs) {
    return Time_Delta(static_cast<float>(lhs) * rhs);
}

Time_Delta operator*(float lhs, Time_Delta const& rhs) {
    return Time_Delta(lhs * static_cast<float>(rhs));
}

std::ostream& operator<<(std::ostream& os, Time_Delta const& val) {
    return os << static_cast<float>(val);
}

//------------------------------------------------------------------------------
// Time

Time_Delta operator-(Time const& lhs, Time const& rhs) {
    return Time_Delta(static_cast<float>(lhs) - static_cast<float>(rhs));
}

Time operator+(Time const& lhs, Time_Delta const& rhs) {
    return Time(static_cast<float>(lhs) + static_cast<float>(rhs));
}

float operator/(Time const& lhs, Time const& rhs)
{
    return static_cast<float>(lhs) / static_cast<float>(rhs);
}

std::ostream& operator<<(std::ostream& os, Time const& val) {
    return os << static_cast<float>(val);
}

//------------------------------------------------------------------------------
// Time_Interval

Time_Delta Time_Interval::delta() const noexcept {
    return end - start;
}

std::ostream& operator<<(std::ostream& os, Time_Interval const& interval)
{
    return os << "[" << interval.start << "," << interval.end << "]";
}
