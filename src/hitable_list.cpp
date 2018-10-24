#include "hitable_list.h"

void Hitable_List::add(std::unique_ptr<Hitable> hitable) {
    m_list.push_back(std::move(hitable));
}

bool Hitable_List::hit(Ray const& r, float t_min, float t_max, Hit_Record& rec) const {
    bool is_hit = false;
    float closest_hit = t_max;
    Hit_Record hit_rec;

    for (auto const& hitable : m_list) {
        if (hitable->hit(r, t_min, closest_hit, hit_rec)) {
            is_hit = true;
            closest_hit = hit_rec.t;
            rec = hit_rec;
        }
    }

    return is_hit;
}

bool Hitable_List::empty() const {
    return m_list.empty();
}
