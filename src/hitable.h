#pragma once

#include "material/material.h"
#include "math/ray.h"

class Material; // TODO(sdsmith): 

struct Hit_Record {
    float t = 0.0f;
    Vec3 p;
    Vec3 normal;
    Material* material = nullptr;
};

class Hitable
{
public:
    virtual bool hit(Ray const& r, float t_min, float t_max, Hit_Record& rec) const = 0;
};
