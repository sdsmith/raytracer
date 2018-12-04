#include "moving_sphere.h"

#include "utility/utility.h"

#include <cassert>
#include <cmath>

bool Moving_Sphere::hit(Ray const& r, float t_min, float t_max, Hit_Record& rec) const {
    Vec3 const center = get_center(r.time);
    Vec3 const oc = r.origin() - center;
    float const a = dot(r.direction(), r.direction());
    float const b = 2.0f * dot(oc, r.direction());
    float const c = dot(oc, oc) - radius * radius;
    float const discriminant = b * b - 4 * a * c;

    if (discriminant < 0) { return false; }

    // Only consider the negative root (should be closer to the viewport)
    float const root = (-b - std::sqrt(discriminant)) / (2.0f * a);

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

Vec3 Moving_Sphere::get_center(Time time) const {
    Time const t0 = lerp_interval.start;
    Time const t1 = lerp_interval.end;

    return center + ((time - t0) / (t1 - t0)) * (center1 - center);
}
