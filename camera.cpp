#include "camera.h"

Ray Camera::to_viewport(float u, float v) const {
    return Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
}
