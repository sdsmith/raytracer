#include "light.h"

#include "utility/utility.h"

#include <cassert>
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
    assert(float_eq(n.length(), 1.0f));

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
    /*
     * Let theta be the angle between the incident ray and the surface normal.
     * Let n1,n2 be the indices of refaction of the two surfaces, where n1 is
     *   assumed to be air (n1 = 1).
     * Let r0 is the reflection coefficient for light incoming parallel to the
     *   normal.
     *
     * Then
     *   R(theta) = r0 + (1 - r0)(1 - cos(theta))^5
     *   where
     *     r0 = ((n1 - n2) / (n1 + n2))^2 = ((1 - n2) / (1 + n2))^2
     */

    float r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}
