#pragma once

#include "hitable.h"
#include "light.h"
#include "math/ray.h"

struct Hit_Record;

class Material
{
public:
    virtual ~Material() {}
    
    /**
     * @return True if scatter ray was generated.
     */
    virtual bool scatter(Ray const& r_in, Hit_Record const& rec, Vec3& attenuation, Ray& scattered) const = 0;    
};
