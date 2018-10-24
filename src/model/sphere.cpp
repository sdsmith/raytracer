#include "sphere.h"

#include "utility/utility.h"

#include <cassert>
#include <cmath>

bool Sphere::hit(Ray const& r, float t_min, float t_max, Hit_Record& rec) const
{
    Vec3 const oc = r.origin() - center;
    float const a = dot(r.direction(), r.direction());
    float const b = 2.0f * dot(oc, r.direction());
    float const c = dot(oc, oc) - radius * radius;
    float const discriminant = b * b - 4 * a * c;
    
    float root;
    if (discriminant > 0) {
        root = (-b - std::sqrt(discriminant)) / (2.0f * a);
    } else {
        root = (-b + std::sqrt(discriminant)) / (2.0f * a);
    }

    if (t_min < root && root < t_max) {
        rec.t = root;
        rec.p = r.point(rec.t);
        rec.normal = (rec.p - center) / radius; // normalized
        assert(float_eq(rec.normal.length(), 1.0f));
        rec.material = material.get();
        return true;
    }

    return false;
}
