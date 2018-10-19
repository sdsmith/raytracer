#pragma once

#include "scene.h"

class Test_Scene : public Scene {
public:
    char const* name() const override;
    std::vector<Hitable*> const& generate() override;
};
