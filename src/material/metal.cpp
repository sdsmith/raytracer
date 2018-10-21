#include "metal.h"

#include "light.h"
#include "utility/utility.h"

bool Metal::scatter(Ray const& r_in, Hit_Record const& rec, Vec3& attenuation, Ray& scattered) const {
    Vec3 const reflected = reflect(unit_vector(r_in.direction()), rec.normal);
    scattered = Ray(rec.p, reflected + fuzz * rand_in_unit_sphere());
    attenuation = albedo;
    return dot(scattered.direction(), rec.normal) > 0;
}
