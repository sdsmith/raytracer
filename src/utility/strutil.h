#pragma once

#include <cstdarg>
#include <string>
#include <vector>

namespace Str_Util
{
    // Ref: https://stackoverflow.com/questions/69738/c-how-to-get-fprintf-results-as-a-stdstring-w-o-sprintf#69911
    std::string format(char const* fmt, ...);
    std::string vformat(char const* fmt, va_list ap);
}
