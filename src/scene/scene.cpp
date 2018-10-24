#include "scene.h"

#include <cassert>

Hitable_List const& Scene::get() const {
    assert(!m_scene.empty());
    return m_scene;
}

bool Scene::empty() const {
    return m_scene.empty();
}
