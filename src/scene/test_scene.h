#pragma once

#include "scene.h"

class Test_Scene : public Scene {
public:
    Test_Scene() : Scene("test") {}
    void generate() override;
};
