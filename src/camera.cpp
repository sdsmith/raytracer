#include "camera.h"

/**
 * @param vertFov Top to bottom in degrees
 * @param aspect Aspect ratio.
 */
Camera::Camera(Ray eye, Vec3 up, float vert_fov, float aspect, float aperture, float focus_dist)
{
    const float theta = vert_fov * pi<float> / 180.0f;
    const float half_height = tan(theta / 2);
    const float half_width = aspect * half_height;

    origin = eye.origin();
    w = unit_vector(origin - eye.direction());
    u = unit_vector(cross(up, w));
    v = cross(w, u);

    // fix viewport 1 unit from camera
    lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
    horizontal = 2 * half_width * focus_dist * u;
    vertical = 2 * half_height * focus_dist * v;

    lens_radius = aperture / 2;
}

Ray Camera::to_viewport(float s, float t) const {
    Vec3 lens_pos = lens_radius * rand_in_unit_disk();
    Vec3 offset = u * lens_pos.x() + v * lens_pos.y();
    return Ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
}
