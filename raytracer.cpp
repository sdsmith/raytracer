#include "camera.h"
#include "hitable.h"
#include "hitable_list.h"
#include "ray.h"
#include "utility.h"
#include "vec3.h"

// Objects
#include "sphere.h"

// Materials
#include "dielectric.h"
#include "lambertian.h"
#include "metal.h"

#ifdef _MSC_VER
// Ignore warnings for unsafe std::gmtime
#pragma warning(disable:4996)
#endif

#include <cassert>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ctime>   // std::gmtime
#include <iomanip> // std::put_time
#include <limits>
#include <memory>
#include <fstream>
#include <vector>

struct Viewport {
    int width;  /** Pixel width */
    int height; /** Pixel height */

    float aspect_ratio() const {
        return width / static_cast<float>(height);
    }
};

Vec3 color(const Ray& r, const Hitable* world, int depth) {
    Hit_Record rec;

    constexpr int max_depth = 50;

    if (world->hit(r, 0.0f + flop_err_thresh, std::numeric_limits<float>::max(), rec)) {
        assert(rec.material);

        Ray scattered;
        Vec3 attenuation;
        if (depth < max_depth && rec.material->scatter(r, rec, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth + 1);
        } else {
            return Vec3(0.0f, 0.0f, 0.0f);
        }
    } else {
        // Background
        Vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5f * (unit_direction.y() + 1.0f); // [-1, 1] -> [0, 1]
        return lerp(Vec3(1.0f, 1.0f, 1.0f), Vec3(0.5f, 0.7f, 1.0f), t);
    }
}

void antialias(const Viewport& viewport, const Camera& cam, const Hitable* world, int pix_u, int pix_v, Vec3& col) {
    constexpr int sample_size = 100;
    
    for (int s = 0; s < sample_size; ++s) {
        float u = static_cast<float>(pix_u + rand_normalized()) / static_cast<float>(viewport.width);
        float v = static_cast<float>(pix_v + rand_normalized()) / static_cast<float>(viewport.height);
        Ray r = cam.to_viewport(u, v);
        Vec3 p = r.point(2.0f); // TODO: Should this be a constant?
        col += color(r, world, 0);
    }

    col /= static_cast<float>(sample_size);
}

Vec3 gamma_correction(const Vec3& col) {
    // Use Gamma 2 (1/gamma => 1/2)

    return Vec3(sqrt(col.r()), sqrt(col.g()), sqrt(col.b()));
}

void gen_ppm(const Viewport& viewport, const Camera& cam, const Hitable* world)
{
    std::ofstream imageFile("image.ppm", std::ios::out | std::ios::trunc);
    if (!imageFile.is_open()) {
        printf("Unable to open output file\n");
        return;
    }

    imageFile << "P3\n" << viewport.width << " " << viewport.height << "\n255\n";

    // Traverse from the viewport's lower left corner to top right corner, 
    // left to right, bottom to top.
    for (int j = viewport.height - 1; j >= 0; --j) {
        for (int i = 0; i < viewport.width; ++i) {
            Vec3 col(0.0f, 0.0f, 0.0f);

            antialias(viewport, cam, world, i, j, col);
            assert(!is_nan(col));

            col = gamma_correction(col);
            assert(!is_nan(col));

            int ir = static_cast<int>(255.99f * col.r());
            int ig = static_cast<int>(255.99f * col.g());
            int ib = static_cast<int>(255.99f * col.b());

            imageFile << ir << " " << ig << " " << ib << "\n";
        }
    }

    imageFile.close();
}

std::vector<Hitable*> random_scene() {
    std::vector<Hitable*> list;

    list.emplace_back(
        new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian({0.5, 0.5, 0.5})));

    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            const float choose_mat = rand_normalized();
            Vec3 center(a + 0.9f * rand_normalized(), 0.2f, b + 0.9f * rand_normalized());
            if ((center - Vec3(4.0f, 0.2f, 0.0f)).length() > 0.9f) {
                if (choose_mat < 0.8f) {
                    // diffuse
                    list.emplace_back(new Sphere(
                        center, 0.2f,
                        new Lambertian(
                            {rand_normalized() * rand_normalized(),
                             rand_normalized() * rand_normalized(),
                             rand_normalized() * rand_normalized()})));
                } else if (choose_mat < 0.95f) {
                    // metal
                    list.emplace_back(
                        new Sphere(center, 0.2f,
                                   new Metal({0.5f * (1 + rand_normalized()),
                                              0.5f * (1 + rand_normalized()),
                                              0.5f * (1 + rand_normalized())},
                                             0.5f * rand_normalized())));
                } else {
                    // glass
                    list.emplace_back(
                        new Sphere(center, 0.2f, new Dielectric(1.5)));
                }
            }
        }
    }

    list.emplace_back(new Sphere({0.0f, 1.0f, 0.0f}, 1.0f, new Dielectric(1.5f)));
    list.emplace_back(
        new Sphere({-4.0f, 1.0f, 0.0f}, 1.0f, new Lambertian({0.4f, 0.2f, 0.1f})));
    list.emplace_back(
        new Sphere({4.0f, 1.0f, 0.0f}, 1.0f, new Metal({0.7f, 0.6f, 0.5f}, 0.0f)));

    return list;
}

std::vector<Hitable*> test_scene()
{
    std::vector<Hitable*> list;

    list.emplace_back(
        new Sphere({0.0f, -100.5f, -1.0f}, 100.0f,
                   new Lambertian({0.8f, 0.8f, 0.0f}))); // base
    {   
        // glass sphere
        list.emplace_back(
            new Sphere({-1.0f, 0.0f, -1.0f}, 0.5f, new Dielectric(1.5f)));
        list.emplace_back(
            new Sphere({-1.0f, 0.0f, -1.0f}, -0.45f, new Dielectric(1.5f)));
    }
    list.emplace_back(new Sphere({0.0f, 0.0f, -1.0f}, 0.5f,
                                 new Lambertian({0.1f, 0.2f, 0.5f})));
    list.emplace_back(new Sphere({1.0f, 0.0f, -1.0f}, 0.5f,
                                 new Metal({0.8f, 0.6f, 0.2f}, 1.0f)));

    return list;
}

int main()
{
    srand(0);

    std::vector<Hitable*> hitables = random_scene();
    std::unique_ptr<Hitable_List> world = std::make_unique<Hitable_List>(hitables.data(), hitables.size());

    constexpr Viewport viewport = {200, 100};
    Vec3 eye_origin(3, 3, 2);
    Vec3 eye_fwd(0, 0, -1);
    float dist_to_focus = (eye_origin - eye_fwd).length();
    float aperture = 2.0f;
    Camera cam(Ray(eye_origin, eye_fwd), 
                   {0, 1, 0}/*up*/, 
                   20/*degree vertFov*/, 
                   viewport.aspect_ratio(),
                   aperture,
                   dist_to_focus);

    auto time_point_start = std::chrono::system_clock::now();
    std::time_t t_start = std::chrono::system_clock::to_time_t(time_point_start);
    std::cout << "Start time: "
              << std::put_time(std::gmtime(&t_start), "%F %T UTC")
              << std::endl;
    {
        gen_ppm(viewport, cam, world.get());
    }
    auto time_point_end = std::chrono::system_clock::now();
    std::time_t t_end = std::chrono::system_clock::to_time_t(time_point_end);
    std::cout << "End time  : "
              << std::put_time(std::gmtime(&t_end), "%F %T UTC")
              << std::endl;

    std::cout << "Duration  : "
              << std::chrono::duration_cast<std::chrono::seconds>(
                     time_point_end - time_point_start).count()
              << "s" << std::endl;

    // TODO: cleanup memory

    return 0;
}

