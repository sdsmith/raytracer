#pragma once

#include "scene.h"

class Random_Scene : public Scene {
public:
    char const* name() const override;
    std::vector<Hitable*> const& generate() override;
};
