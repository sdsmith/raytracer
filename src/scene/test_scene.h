#pragma once

#include "scene.h"

class Test_Scene : public Scene {
public:
    char const* name() const override;
    void generate() override;
};
