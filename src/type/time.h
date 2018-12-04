#pragma once

#include <cmath>
#include <ostream>

#include <type_safe/strong_typedef.hpp>

//using Time = float;
struct Time_Delta
    : type_safe::strong_typedef<Time_Delta, float>,
      type_safe::strong_typedef_op::relational_comparison<Time_Delta>
{
    using strong_typedef::strong_typedef;

    friend float operator/(Time_Delta const& lhs, Time_Delta const& rhs);
    friend Time_Delta operator*(Time_Delta const& lhs, float rhs);
    friend Time_Delta operator*(float lhs, Time_Delta const& rhs);
    friend std::ostream& operator<<(std::ostream& os, Time_Delta const& val);
};

struct Time
    : type_safe::strong_typedef<Time, float>,
      type_safe::strong_typedef_op::relational_comparison<Time>,
      type_safe::strong_typedef_op::unary_plus<Time>,
      type_safe::strong_typedef_op::unary_minus<Time>,
      type_safe::strong_typedef_op::addition<Time>
{
    using strong_typedef::strong_typedef;

    friend Time_Delta operator-(Time const& lhs, Time const& rhs);
    friend Time operator+(Time const& lhs, Time_Delta const& rhs);
    friend float operator/(Time const& lhs, Time const& rhs);
    friend std::ostream& operator<<(std::ostream& os, Time const& val);
};

constexpr Time INVALID_TIME(NAN);

struct Time_Interval {
public:
    Time start = INVALID_TIME;
    Time end = INVALID_TIME;

    Time_Interval() {}
    Time_Interval(Time start, Time end) : start(start), end(end) {}
    Time_Delta delta() const noexcept;

    friend std::ostream& operator<<(std::ostream& os, Time_Interval const& interval);
};
