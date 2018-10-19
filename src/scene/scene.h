#pragma once

#include "hitable.h"

#include <vector>

class Scene {
public:
    virtual char const* name() const = 0;
    virtual std::vector<Hitable*> const& generate() = 0;
    std::vector<Hitable*> const& get() const;

protected:
    std::vector<Hitable*> m_scene;
};
