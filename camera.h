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
    Camera(float vertFov, float aspect) {
        const float theta = vertFov * pi<float> / 180.0f;
        const float half_height = tan(theta / 2);
        const float half_width = aspect * half_height;
        lower_left_corner = Vec3(-half_width, -half_height, -1.0f); // fix camera 1 unit from the viewport
        horizontal = Vec3(2 * half_width, 0.0f, 0.0f);
        vertical = Vec3(0.0f, 2 * half_height, 0.0f);
        origin = Vec3(0.0f, 0.0f, 0.0f);
    }

    Ray to_viewport(float u, float v) const;

    Vec3 origin;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 lower_left_corner;
};

