#include "utility.h"

#include <cstdlib> // rand

Vec3 lerp(Vec3 const& start, Vec3 const& end, float t) {
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

Vec3 rand_in_unit_disk() {
    Vec3 p;
    do {
        p = 2.0f * Vec3(rand_normalized(), rand_normalized(), 0) -
            Vec3(1.0f, 1.0f, 0.0f);
    } while (dot(p, p) >= 1.0f);

    return p;
}
