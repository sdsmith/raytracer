#pragma once

#include "ray.h"
#include "utility.h"

class Camera
{
public:
    Camera(Ray eye, Vec3 up, float vert_fov, float aspect, float aperture,
           float focus_disk);

    Ray to_viewport(float u, float v) const;

private:
    Vec3 origin;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 lower_left_corner;

    Vec3 u, v, w; // orthogonal basis
    float lens_radius;
};

