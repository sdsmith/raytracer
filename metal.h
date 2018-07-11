#pragma once

#include "behaviour.h"
#include "material.h"

class Metal : public Material
{
public:
    Metal(const Vec3& albedo) : albedo(albedo) {}

    virtual bool scatter(const Ray& r_in, const Hit_Record& rec, Vec3& attenuation, Ray& scattered) const {
        Vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = Ray(rec.p, reflected);
        attenuation = albedo;
        return dot(scattered.direction(), rec.normal) > 0;
    }

    Vec3 albedo;
};

