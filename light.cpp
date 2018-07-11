#include "light.h"

#include <cmath>

/**
 * @param v Incident vector.
 * @param n Hitpoint surface normal.
 */
Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - 2 * dot(v, n) * n;
}

/**
* @param v Incident vector.
* @param n Hitpoint surface normal.
*/
bool refract(const Vec3& v, const Vec3& n, float ni, float nt, Vec3& refracted) {
    /*
     * Using Snell's law. 
     */

    const Vec3 uv = unit_vector(v);
    const float dt = dot(uv, n);
    const float ni_over_nt = ni / nt;
    const float discriminant = 1.0f - ni_over_nt * ni_over_nt * (1.0f - dt * dt);
    if (discriminant > 0) {
        refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
        return true;
    } else {
        return false;
    }
}

/**
 * @brief Glass angle reflectivity approximation (Christophe Schlick).
 */
float schlick(float cosine, float refraction_index) {
    float r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}