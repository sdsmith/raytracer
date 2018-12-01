#include "camera.h"

/**
 * @param vertFov Top to bottom in degrees
 * @param aspect Aspect ratio.
 */
Camera::Camera(Ray eye, Vec3 up, float vert_fov, float aspect, float aperture, float focus_dist, Time_Interval time_window)
    : m_shutter_window(time_window)
{
    float const theta = vert_fov * pi<float> / 180.0f;
    float const half_height = static_cast<float>(tan(theta / 2.0f)); // TODO(sdsmith): down cast
    float const half_width = aspect * half_height;

    m_origin = eye.origin();
    m_w = unit_vector(m_origin - eye.direction());
    m_u = unit_vector(cross(up, m_w));
    m_v = cross(m_w, m_u);

    // fix viewport 1 unit from camera
    m_lower_left_corner = m_origin - half_width * focus_dist * m_u - half_height * focus_dist * m_v - focus_dist * m_w;
    m_horizontal = 2 * half_width * focus_dist * m_u;
    m_vertical = 2 * half_height * focus_dist * m_v;

    m_lens_radius = aperture / 2;
}

Ray Camera::to_viewport(float s, float t) const {
    Vec3 const lens_pos = m_lens_radius * rand_in_unit_disk();
    Vec3 const offset = m_u * lens_pos.x() + m_v * lens_pos.y();
    Time const time = m_shutter_window.start + static_cast<float>(rand()) * m_shutter_window.delta();

    return Ray(m_origin + offset,
               m_lower_left_corner + s * m_horizontal + t * m_vertical - m_origin - offset,
               time);
}
