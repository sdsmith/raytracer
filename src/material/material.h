#pragma once

#include "hitable.h"
#include "light.h"
#include "math/ray.h"

struct Hit_Record;

class Material
{
public:
    /**
     * @return True if scatter ray was generated.
     */
    virtual bool scatter(const Ray& r_in, const Hit_Record& rec, Vec3& attenuation, Ray& scattered) const = 0;
};
