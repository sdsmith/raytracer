#include "test_scene.h"

#include "material/dielectric.h"
#include "material/lambertian.h"
#include "material/metal.h"
#include "model/sphere.h"

void Test_Scene::generate() {
    if (!m_scene.empty()) {
        return;
    }

    // base
    m_scene.add(std::make_unique<Sphere>(Vec3(0.0f, -100.5f, -1.0f), 100.0f,
                                         std::make_unique<Lambertian>(Vec3(0.8f, 0.8f, 0.0f))));

    { // glass sphere
        m_scene.add(std::make_unique<Sphere>(Vec3(-1.0f, 0.0f, -1.0f), 0.5f,
                                             std::make_unique<Dielectric>(1.5f)));
        m_scene.add(std::make_unique<Sphere>(Vec3(-1.0f, 0.0f, -1.0f), -0.45f,
                                             std::make_unique<Dielectric>(1.5f)));
    }

    m_scene.add(std::make_unique<Sphere>(Vec3(0.0f, 0.0f, -1.0f), 0.5f,
                                         std::make_unique<Lambertian>(Vec3(0.1f, 0.2f, 0.5f))));

    m_scene.add(std::make_unique<Sphere>(Vec3(1.0f, 0.0f, -1.0f), 0.5f,
                                         std::make_unique<Metal>(Vec3(0.8f, 0.6f, 0.2f), 1.0f)));
}
