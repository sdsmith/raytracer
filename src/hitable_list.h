#pragma once

#include "hitable.h"

#include <memory>
#include <vector>

class Hitable_List : public Hitable
{
public:
    Hitable_List() {}    
    ~Hitable_List() override {}

    void add(std::unique_ptr<Hitable> hitable);
    bool hit(Ray const& r, float t_min, float t_max, Hit_Record& rec) const override;
    bool empty() const;

private:
    std::vector<std::unique_ptr<Hitable>> m_list = {};
};

