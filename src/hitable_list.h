#pragma once

#include "hitable.h"

class Hitable_List : public Hitable
{
public:
    Hitable_List() {};
    Hitable_List(std::vector<Hitable*> hitables) : m_list(hitables) {}
    void add(Hitable* hitable) { m_list.emplace_back(hitable); }
    bool hit(const Ray& r, float t_min, float t_max, Hit_Record& rec) const override;

private:
    std::vector<Hitable*> m_list;
};

