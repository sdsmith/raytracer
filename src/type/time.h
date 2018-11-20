#pragma once

using Time = float;

struct Time_Interval {
public:
    Time start;
    Time end;

    Time delta() const;
};
