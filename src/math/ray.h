#pragma once

#include "vec3.h"

#include <ostream>

struct Ray
{
public:
    Ray() {};
    Ray(Vec3 const& p1, Vec3 const& p2) : p1(p1), p2(p2) {}

    Vec3 origin() const;
    Vec3 direction() const;
    Vec3 point(float t) const;

    Vec3 p1 = {};
    Vec3 p2 = {};
};

std::ostream& operator<<(std::ostream& os, Ray const& ray);
