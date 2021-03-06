#include "dielectric.h"

#include <cassert>

bool Dielectric::scatter(Ray const& r_in, Hit_Record const& rec, Vec3& attenuation, Ray& scattered) const {
    /**
     * Assuming all light is refracted when hitting material with higher
     * refractive index.
     */

    // Glass surface absorbs nothing (attinuation = 1)
    attenuation = Vec3(1.0f, 1.0f, 1.0f);

    Vec3 outward_normal;
    float ni, nt;
    float cosine;
    if (dot(r_in.direction(), rec.normal) > 0) {
        outward_normal = -rec.normal;
        ni = m_refractive_index;
        nt = 1.0f;
        cosine = m_refractive_index * dot(r_in.direction(), rec.normal) / r_in.direction().length();
    } else {
        outward_normal = rec.normal;
        ni = 1.0f;
        nt = m_refractive_index;
        cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
    }

    // Reflection probability
    Vec3 refracted;
    float reflect_prob;
    if (refract(r_in.direction(), outward_normal, ni, nt, refracted)) {
        // Because it's dielectric, assume there is a chance of reflection
        // based on schlick's approx rather than guaranteed refraction.
        reflect_prob = schlick(cosine, m_refractive_index);
    } else {
        reflect_prob = 1.0f;
    }
    assert(0.0f <= reflect_prob && reflect_prob <= 1.0f);

    if (rand_normalized() <= reflect_prob) {
        scattered = Ray(rec.p, reflect(r_in.direction(), rec.normal), r_in.time);
    } else {
        scattered = Ray(rec.p, refracted, r_in.time);
    }

    return true;
}
