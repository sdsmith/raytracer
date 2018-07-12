#pragma once

#include "vec3.h"

// Packed attribute
//
#if defined(__GNUC__) // __GNUG__
#   define PACK(__Declaration__) __Declaration__ __attribute__((__packed__))
#else
#if (defined(_WIN32) || defined(_WIN64)) && defined(_MSC_FULL_VER)
#   define PACK(__Declaration__) __pragma(pack(push, 1)) __Declaration__ __pragma(pack(pop))
#else
#   error "Cannot determine packed attribute: unknown compiler"
#endif
#endif

constexpr float flop_err_thresh = 0.00001f;

constexpr bool float_eq(float a, float b) {
    return (a - flop_err_thresh) < b && b < (a + flop_err_thresh);
}

Vec3 lerp(const Vec3& start, const Vec3& end, float t);

float rand_normalized();

Vec3 rand_in_unit_sphere();

inline bool is_nan(float f) { return f != f; }
inline bool is_nan(double f) { return f != f; }
inline bool is_nan(const Vec3& v) {
    return is_nan(v.e[0]) || is_nan(v.e[1]) || is_nan(v.e[2]);
}