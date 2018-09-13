#pragma once

#include "ray.h"
#include "utility.h"

class Camera
{
public:
    /**
     * @param vertFov Top to bottom in degrees
     * @param aspect Aspect ratio.
     */
    Camera(Ray eye, Vec3 up, float vertFov, float aspect) {
        Vec3 u, v, w; // orthogonal basis

        const float theta = vertFov * pi<float> / 180.0f;
        const float half_height = tan(theta / 2);
        const float half_width = aspect * half_height;

        origin = eye.origin();
        w = unit_vector(origin - eye.direction());
        u = unit_vector(cross(up, w));
        v = cross(w, u);

        // fix viewport 1 unit from camera
        lower_left_corner = origin - half_width * u - half_height * v - w;
        horizontal = 2 * half_width * u;
        vertical = 2 * half_height * v;
    }

    Ray to_viewport(float u, float v) const;

    Vec3 origin;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 lower_left_corner;
};

