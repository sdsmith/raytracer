#pragma once

#include "material.h"

/**
 * @brief Lambertian (diffuse) material.
 */
class Lambertian : public Material
{
public:
    Lambertian(Vec3 const& albedo) : albedo(albedo) {}

    virtual bool scatter(Ray const& r_in, Hit_Record const& rec, Vec3& attenuation, Ray& scattered) const;

private:
    Vec3 albedo;
};
