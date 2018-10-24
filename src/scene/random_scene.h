#pragma once

#include "scene.h"

class Random_Scene : public Scene {
public:
    char const* name() const override;
    void generate() override;
};
