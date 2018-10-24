#pragma once

#include "hitable_list.h"

#include <memory>
#include <vector>

class Scene {
public:
    virtual ~Scene() {}
    
    virtual char const* name() const = 0;
    virtual void generate() = 0;
    Hitable_List const& get() const;
    bool empty() const;

protected:
    Hitable_List m_scene = {};
};
