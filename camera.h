#pragma once

#include "ray.h"

class Camera
{
public:
    Camera() 
        : origin(Vec3(0.0f, 0.0f, 0.0f)),
          horizontal(Vec3(4.0f, 0.0f, 0.0f)), 
          vertical(Vec3(0.0f, 2.0f, 0.0f)), 
          lower_left_corner(Vec3(-2.0f, -1.0f, -1.0f))
        {}

    Ray to_viewport(float u, float v) const;

    Vec3 origin;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 lower_left_corner;
};

