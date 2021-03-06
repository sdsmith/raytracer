#pragma once

#include "math/vec3.h"

#include <cmath>

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

constexpr float flop_err_thresh = 0.0005f;

constexpr bool float_eq(float a, float b) {
    // return (a - flop_err_thresh) < b && b < (a + flop_err_thresh);
    return true; // Bad floating point errors. Make it based on ray 
                 // depth to account for compounding error.
}

Vec3 lerp(const Vec3& start, const Vec3& end, float t);

float rand_normalized();

Vec3 rand_in_unit_sphere();
Vec3 rand_in_unit_disk();

template<typename T>
constexpr T pi = static_cast<T>(3.14159265358979323846L);

template<typename T>
struct Point2D {
    T x;
    T y;
};
