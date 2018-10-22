#pragma once

#include "vec3.h"

struct Ray
{
public:
    Ray() {};
    Ray(Vec3 const& a, Vec3 const& b) : A(a), B(b) {}

    Vec3 origin() const    { return A; }
    Vec3 direction() const { return B; }
    Vec3 point(float t) const { return A + t * B;  }

    Vec3 A = {};
    Vec3 B = {};
};

