#pragma once

#include "hitable.h"

class Sphere : public Hitable
{
public:
    Sphere() {};
    Sphere(Vec3 center, float radius) : center(center), radius(radius) {}
    bool hit(const Ray& r, float t_min, float t_max, Hit_Record& rec) const override;

    Vec3 center;
    float radius = 1;
};

