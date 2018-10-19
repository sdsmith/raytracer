#include "test_scene.h"

#include "material/dielectric.h"
#include "material/lambertian.h"
#include "material/metal.h"
#include "model/sphere.h"

char const* Test_Scene::name() const { return "test"; }

std::vector<Hitable*> const& Test_Scene::generate() {
    m_scene.emplace_back(new Sphere({0.0f, -100.5f, -1.0f}, 100.0f,
                                    new Lambertian({0.8f, 0.8f, 0.0f}))); // base
    {
        // glass sphere
        m_scene.emplace_back(
            new Sphere({-1.0f, 0.0f, -1.0f}, 0.5f, new Dielectric(1.5f)));
        m_scene.emplace_back(
            new Sphere({-1.0f, 0.0f, -1.0f}, -0.45f, new Dielectric(1.5f)));
    }
    m_scene.emplace_back(new Sphere({0.0f, 0.0f, -1.0f}, 0.5f,
                                    new Lambertian({0.1f, 0.2f, 0.5f})));
    m_scene.emplace_back(new Sphere({1.0f, 0.0f, -1.0f}, 0.5f,
                                    new Metal({0.8f, 0.6f, 0.2f}, 1.0f)));

    return m_scene;
}
