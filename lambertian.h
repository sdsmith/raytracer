#pragma once

#include "material.h"
#include "utility.h"

/**
 * @brief Lambertian (diffuse) material.
 */
class Lambertian : public Material
{
public:
    Lambertian(const Vec3& albedo) : albedo(albedo) {}

    virtual bool scatter(const Ray& r_in, const Hit_Record& rec, Vec3& attenuation, Ray& scattered) const {
        /*
        * Pick a random point tangent to the hitpoint and shoot another ray. Any
        * collection of unmodified random rays will derive diffuse light.
        */

        Vec3 target = rec.p + rec.normal + rand_in_unit_sphere();
        scattered = Ray(rec.p, target - rec.p);
        attenuation = albedo;
        return true;
    }

    Vec3 albedo;
};
