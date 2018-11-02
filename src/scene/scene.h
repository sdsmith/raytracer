#pragma once

#include "hitable_list.h"

#include <memory>
#include <string>
#include <vector>

class Scene {
public:
    Scene() = default;
    Scene(std::string name) : m_name(name) {}
    virtual ~Scene() {}

    std::string const name() const;
    virtual void generate() = 0;
    Hitable_List const& get() const; // TODO(sdsmith): rename get_hitables
    bool empty() const;

protected:
    Hitable_List m_scene = {};
    std::string m_name = {};
};
