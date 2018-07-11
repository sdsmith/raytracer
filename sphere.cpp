#include "sphere.h"

bool Sphere::hit(const Ray& r, float t_min, float t_max, Hit_Record& rec) const
{
    Vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0f * dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    
    float root;
    if (discriminant > 0) {
        root = (-b - sqrt(discriminant)) / (2.0f * a);
    } else {
        root = (-b + sqrt(discriminant)) / (2.0f * a);
    }

    if (t_min < root && root < t_max) {
        rec.t = root;
        rec.p = r.point(rec.t);
        rec.normal = (rec.p - center) / radius; // normalized
        rec.material = material;
        return true;
    }

    return false;
}