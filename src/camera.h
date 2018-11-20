#pragma once

#include "math/ray.h"
#include "type/time.h"
#include "utility/utility.h"

class Camera
{
public:
    Camera(Ray eye, Vec3 up, float vert_fov, float aspect, float aperture,
           float focus_disk, Time_Interval shutter_window);

    Ray to_viewport(float u, float v) const;

private:
    Vec3 m_origin = {};
    Vec3 m_horizontal = {};
    Vec3 m_vertical = {};
    Vec3 m_lower_left_corner = {};

    // orthogonal basis
    Vec3 m_u = {};
    Vec3 m_v = {};
    Vec3 m_w = {};

    float m_lens_radius = 0;
    Time_Interval m_shutter_window = {};
};
