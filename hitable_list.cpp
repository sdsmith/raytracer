#include "hitable_list.h"

bool Hitable_List::hit(const Ray& r, float t_min, float t_max, Hit_Record& rec) const {
    bool is_hit = false;
    float closest_hit = t_max;
    Hit_Record hit_rec;

    for (size_t i = 0; i < list_size; ++i) {
        if (list[i]->hit(r, t_min, closest_hit, hit_rec)) {
            is_hit = true;
            closest_hit = hit_rec.t;
            rec = hit_rec;
        }
    }

    return is_hit;
}
