#include "time.h"

Time Time_Interval::delta() const { return end - start; }
