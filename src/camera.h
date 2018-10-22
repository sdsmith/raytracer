#pragma once

#include "math/ray.h"
#include "utility/utility.h"

class Camera
{
public:
    Camera(Ray eye, Vec3 up, float vert_fov, float aspect, float aperture,
           float focus_disk);

    Ray to_viewport(float u, float v) const;

private:
    Vec3 origin = {};
    Vec3 horizontal = {};
    Vec3 vertical = {};
    Vec3 lower_left_corner = {};

    // orthogonal basis
    Vec3 u = {};
    Vec3 v = {};
    Vec3 w = {};
    
    float lens_radius = 0;
};

