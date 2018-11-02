#include "ray.h"

Vec3 Ray::origin() const       { return p1; }

Vec3 Ray::direction() const    { return p2; }

Vec3 Ray::point(float t) const { return p1 + t * p2;  }

std::ostream& operator<<(std::ostream& os, Ray const& ray) {
    return os << ray.p1 << ", " << ray.p2;
}
