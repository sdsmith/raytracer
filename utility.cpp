#include "utility.h"

#include <cstdlib>

Vec3 lerp(const Vec3& start, const Vec3& end, float t) {
    return (1.0f - t) * start + t * end;
}

float rand_normalized() {
    return rand() / static_cast<float>(RAND_MAX);
}

Vec3 rand_in_unit_sphere() {
    Vec3 p;
    do {
        p = 2.0f * Vec3(rand_normalized(), rand_normalized(), rand_normalized()) - Vec3(1.0f, 1.0f, 1.0f);
    } while (p.squared_length() >= 1.0f);

    return p;
}