#include "scene.h"

#include <cassert>

std::vector<Hitable*> const& Scene::get() const {
    assert(!m_scene.empty());
    return m_scene;
}
