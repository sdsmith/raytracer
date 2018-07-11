#pragma once

#include "behaviour.h"
#include "hitable.h"
#include "ray.h"

struct Hit_Record;

class Material
{
public:
    virtual bool scatter(const Ray& r_in, const Hit_Record& rec, Vec3& attenuation, Ray& scattered) const = 0;
};
