#pragma once

#include "hitable.h"
#include "material/material.h"
#include "math/ray.h"
#include "math/vec3.h"
#include "sphere.h"
#include "type/time.h"

#include <memory>

class Moving_Sphere : public Sphere
{
public:
    Moving_Sphere(Vec3 center0, Vec3 center1, float radius, std::unique_ptr<Material>  material, Time_Interval lerp_interval)
        : Sphere(center0, radius, std::move(material)), center1(center1), lerp_interval(lerp_interval) {}

    bool hit(Ray const& r, float t_min, float t_max, Hit_Record& rec) const override;
    Vec3 get_center(Time time) const;

    Vec3 center1 = {};
    Time_Interval lerp_interval;
};
