#pragma once

#include "hitable.h"
#include "material/material.h"
#include "math/ray.h"
#include "math/vec3.h"

struct Sphere : public Hitable
{
public:
    Sphere() : radius(1), material(nullptr) {}
    Sphere(Vec3 center, float radius, Material* material) 
        : center(center), radius(radius), material(material) {}
    Sphere(Sphere const& o) = default;
    
    bool hit(Ray const& r, float t_min, float t_max, Hit_Record& rec) const override;

    Sphere& operator=(Sphere const& o) = default;
    
    Vec3 center = {};
    float radius = 0;
    Material* material = nullptr;
};

