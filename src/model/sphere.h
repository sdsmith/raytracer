#pragma once

#include "hitable.h"
#include "material/material.h"
#include "math/ray.h"
#include "math/vec3.h"

#include <memory>

struct Sphere : public Hitable
{
public:
    Sphere(Vec3 center, float radius, std::unique_ptr<Material> material) 
        : center(center), radius(radius), material(std::move(material)) {}
    Sphere(Sphere const& o) = default;
    
    bool hit(Ray const& r, float t_min, float t_max, Hit_Record& rec) const override;

    Sphere& operator=(Sphere const& o) = default;
    
    Vec3 center = {};
    float radius = 1;
    std::unique_ptr<Material> material = nullptr;
};

