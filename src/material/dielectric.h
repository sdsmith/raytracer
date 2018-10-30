#pragma once

#include "material.h"
#include "light.h"
#include "utility/utility.h"

class Dielectric : public Material
{
public:
    explicit Dielectric(float refractive_index) : m_refractive_index(refractive_index) {}

    virtual bool scatter(Ray const& r_in, Hit_Record const& rec, Vec3& attenuation, Ray& scattered) const;

private:
    float m_refractive_index;
};
