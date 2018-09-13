#pragma once

#include "hitable.h"

class Hitable_List : public Hitable
{
public:
    Hitable_List() {};
    Hitable_List(Hitable** list, size_t size) : list(list), list_size(size) {}

    bool hit(const Ray& r, float t_min, float t_max, Hit_Record& rec) const override;

private:
    Hitable** list = nullptr;
    size_t list_size = 0;
};

