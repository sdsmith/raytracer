#pragma once

#include "material.h"
#include "ray.h"

class Material;

struct Hit_Record {
    float t = 0.0f;
    Vec3 p;
    Vec3 normal;
    Material* material = nullptr;
};

class Hitable
{
public:
    virtual bool hit(const Ray& r, float t_min, float t_max, Hit_Record& rec) const = 0;
};
