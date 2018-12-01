#include "lambertian.h"

#include "utility/utility.h"

bool Lambertian::scatter(Ray const& r_in, Hit_Record const& rec, Vec3& attenuation, Ray& scattered) const {
    /*
     * Pick a random point tangent to the hitpoint and shoot another ray. Any
     * collection of unmodified random rays will derive diffuse light.
     */

    Vec3 target = rec.p + rec.normal + rand_in_unit_sphere();
    scattered = Ray(rec.p, target - rec.p, r_in.time);
    attenuation = m_albedo;
    return true;
}
