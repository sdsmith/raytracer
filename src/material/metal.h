#pragma once

#include "material.h"

class Metal : public Material
{
public:
    Metal(Vec3 const& albedo, float fuzz) : albedo(albedo), fuzz(fuzz < 1.0f ? fuzz : 1.0f) {}

    virtual bool scatter(Ray const& r_in, Hit_Record const& rec, Vec3& attenuation, Ray& scattered) const;

private:
    Vec3 albedo;
    float fuzz;
};

