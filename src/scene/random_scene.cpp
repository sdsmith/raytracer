#include "random_scene.h"

#include "material/dielectric.h"
#include "material/lambertian.h"
#include "material/metal.h"
#include "model/sphere.h"
#include "utility/utility.h"

char const* Random_Scene::name() const { return "random"; }

std::vector<Hitable*> const& Random_Scene::generate() {
    // If the scene has been generated, return it
    if (!m_scene.empty()) {
        return m_scene;
    }
        
    m_scene.emplace_back(
        new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian({0.5, 0.5, 0.5})));

    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            const float choose_mat = rand_normalized();
            Vec3 center(a + 0.9f * rand_normalized(), 0.2f,
                        b + 0.9f * rand_normalized());
            if ((center - Vec3(4.0f, 0.2f, 0.0f)).length() > 0.9f) {
                if (choose_mat < 0.8f) {
                    // diffuse
                    m_scene.emplace_back(new Sphere(
                                             center, 0.2f,
                                             new Lambertian(
                                                 {rand_normalized() * rand_normalized(),
                                                  rand_normalized() * rand_normalized(),
                                                  rand_normalized() * rand_normalized()})));
                } else if (choose_mat < 0.95f) {
                    // metal
                    m_scene.emplace_back(
                        new Sphere(center, 0.2f,
                                   new Metal({0.5f * (1 + rand_normalized()),
                                              0.5f * (1 + rand_normalized()),
                                              0.5f * (1 + rand_normalized())},
                                       0.5f * rand_normalized())));
                } else {
                    // glass
                    m_scene.emplace_back(
                        new Sphere(center, 0.2f, new Dielectric(1.5)));
                }
            }
        }
    }

    m_scene.emplace_back(
        new Sphere({0.0f, 1.0f, 0.0f}, 1.0f, new Dielectric(1.5f)));
    m_scene.emplace_back(new Sphere({-4.0f, 1.0f, 0.0f}, 1.0f,
                                    new Lambertian({0.4f, 0.2f, 0.1f})));
    m_scene.emplace_back(new Sphere({4.0f, 1.0f, 0.0f}, 1.0f,
                                    new Metal({0.7f, 0.6f, 0.5f}, 0.0f)));

    return m_scene;
}
