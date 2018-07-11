#pragma once

#include "hitable.h"

class Sphere : public Hitable
{
public:
    Sphere() : radius(1), material(nullptr) {}
    Sphere(Vec3 center, float radius, Material* material) 
        : center(center), radius(radius), material(material) {}
    
    bool hit(const Ray& r, float t_min, float t_max, Hit_Record& rec) const override;

    Vec3 center;
    float radius;
    Material* material;
};

