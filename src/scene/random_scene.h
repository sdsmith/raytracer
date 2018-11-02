#pragma once

#include "scene.h"

class Random_Scene : public Scene {
public:
    Random_Scene() : Scene("random") {}
    void generate() override;
};
