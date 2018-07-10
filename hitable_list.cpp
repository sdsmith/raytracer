#include "hitable_list.h"

bool Hitable_List::hit(const Ray& r, float t_min, float t_max, Hit_Record& rec) const {
    bool is_hit = false;
    float closest_hit = t_max;
    Hit_Record test_rec;

    for (size_t i = 0; i < list_size; ++i) {
        if (list[i]->hit(r, t_min, closest_hit, test_rec)) {
            is_hit = true;
            closest_hit = test_rec.t;
            rec = test_rec;
        }
    }

    return is_hit;
}
