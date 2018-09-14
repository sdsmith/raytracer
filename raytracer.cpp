#include "camera.h"
#include "hitable.h"
#include "hitable_list.h"
#include "ray.h"
#include "strutil.h"
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
#pragma warning(disable : 4996)
#endif

#include <cassert>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ctime> // std::gmtime
#include <fstream>
#include <iomanip> // std::put_time
#include <limits>
#include <memory>
#include <sstream>
#include <vector>

struct Viewport {
    int width;  /** Pixel width */
    int height; /** Pixel height */

    float aspect_ratio() const { return width / static_cast<float>(height); }
};

struct Config {
    unsigned int rand_seed; // seed for randomization
    Viewport viewport;
    Ray eye;
    Vec3 up;
    float vert_fov; // vertical field of view (in degrees)
    float aperture;
    int aa_sample_size; // anti-aliasing sample size per pixel
    int max_ray_depth;
};

std::string to_file_format(const char* ln_prefix, const Config& cfg)
{
    return Str_Util::format(
        "%s Seed            : %X\n"
        "%s Window          : %dx%d\n"
        "%s Camera          : origin(%s), fwd(%s), up(%s)\n"
        "%s FOV (vert)      : %f\n"
        "%s Aperture        : %f\n"
        "%s AA sample/px    : %d\n"
        "%s Ray depth (max) : %d\n",
        ln_prefix, cfg.rand_seed, ln_prefix, cfg.viewport.width,
        cfg.viewport.height, ln_prefix, cfg.eye.origin().to_string().c_str(),
        cfg.eye.direction().to_string().c_str(), cfg.up.to_string().c_str(),
        ln_prefix, cfg.vert_fov, ln_prefix, cfg.aperture, ln_prefix,
        cfg.aa_sample_size, ln_prefix, cfg.max_ray_depth);
}

Vec3 color(const Ray& r, const Hitable* world, int depth, int max_depth)
{
    Hit_Record rec;

    if (world->hit(r, 0.0f + flop_err_thresh, std::numeric_limits<float>::max(),
                   rec)) {
        assert(rec.material);

        Ray scattered;
        Vec3 attenuation;
        if (depth < max_depth &&
            rec.material->scatter(r, rec, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth + 1, max_depth);
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

void antialias(const Config& cfg, const Camera& cam, const Hitable* world,
               int pix_u, int pix_v, Vec3& col)
{
    for (int s = 0; s < cfg.aa_sample_size; ++s) {
        float u = static_cast<float>(pix_u + rand_normalized()) /
                  static_cast<float>(cfg.viewport.width);
        float v = static_cast<float>(pix_v + rand_normalized()) /
                  static_cast<float>(cfg.viewport.height);
        Ray r = cam.to_viewport(u, v);
        Vec3 p = r.point(2.0f); // TODO: Should this be a constant?
        col += color(r, world, 0, cfg.max_ray_depth);
    }

    col /= static_cast<float>(cfg.aa_sample_size);
}

Vec3 gamma_correction(const Vec3& col)
{
    // Use Gamma 2 (1/gamma => 1/2)

    return Vec3(sqrt(col.r()), sqrt(col.g()), sqrt(col.b()));
}

void gen_ppm(const Config& cfg, const Hitable* world)
{
    const Viewport& viewport = cfg.viewport;
    const float dist_to_focus =
        (cfg.eye.origin() - cfg.eye.direction()).length();
    Camera cam(cfg.eye, cfg.up, cfg.vert_fov, cfg.viewport.aspect_ratio(),
               cfg.aperture, dist_to_focus);

    std::ofstream imageFile("image.ppm", std::ios::out | std::ios::trunc);
    if (!imageFile.is_open()) {
        printf("Unable to open output file\n");
        return;
    }

    imageFile << to_file_format("#", cfg);
    imageFile << "P3\n"
              << viewport.width << " " << viewport.height << "\n255\n";

    // Traverse from the viewport's lower left corner to top right corner,
    // left to right, bottom to top.
    for (int j = viewport.height - 1; j >= 0; --j) {
        for (int i = 0; i < viewport.width; ++i) {
            Vec3 col(0.0f, 0.0f, 0.0f);

            antialias(cfg, cam, world, i, j, col);
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

std::vector<Hitable*> random_scene()
{
    std::vector<Hitable*> list;

    list.emplace_back(
        new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian({0.5, 0.5, 0.5})));

    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            const float choose_mat = rand_normalized();
            Vec3 center(a + 0.9f * rand_normalized(), 0.2f,
                        b + 0.9f * rand_normalized());
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

    list.emplace_back(
        new Sphere({0.0f, 1.0f, 0.0f}, 1.0f, new Dielectric(1.5f)));
    list.emplace_back(new Sphere({-4.0f, 1.0f, 0.0f}, 1.0f,
                                 new Lambertian({0.4f, 0.2f, 0.1f})));
    list.emplace_back(new Sphere({4.0f, 1.0f, 0.0f}, 1.0f,
                                 new Metal({0.7f, 0.6f, 0.5f}, 0.0f)));

    return list;
}

std::vector<Hitable*> test_scene()
{
    std::vector<Hitable*> list;

    list.emplace_back(new Sphere({0.0f, -100.5f, -1.0f}, 100.0f,
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

void raytrace(const Config& cfg)
{
    srand(cfg.rand_seed);

    std::vector<Hitable*> hitables = test_scene(); // random_scene();
    std::unique_ptr<Hitable_List> world =
        std::make_unique<Hitable_List>(hitables.data(), hitables.size());

    auto time_point_start = std::chrono::system_clock::now();
    std::time_t t_start =
        std::chrono::system_clock::to_time_t(time_point_start);
    std::cout << "Start time: "
              << std::put_time(std::gmtime(&t_start), "%F %T UTC") << std::endl;
    {
        gen_ppm(cfg, world.get());
    }
    auto time_point_end = std::chrono::system_clock::now();
    std::time_t t_end = std::chrono::system_clock::to_time_t(time_point_end);
    std::cout << "End time  : "
              << std::put_time(std::gmtime(&t_end), "%F %T UTC") << std::endl;

    std::cout << "Duration  : "
              << std::chrono::duration_cast<std::chrono::seconds>(
                     time_point_end - time_point_start)
                     .count()
              << "s" << std::endl;

    // TODO: cleanup memory
}

int main()
{
    Config cfg = {};
    cfg.rand_seed = 0;
    cfg.viewport = {1920, 1080}; //{200, 100};
    cfg.eye = {{3, 3, 2}, {0, 0, -1}};
    cfg.up = {0, 1, 0};
    cfg.vert_fov = 20;
    cfg.aperture = 2.0f;
    cfg.aa_sample_size = 100;
    cfg.max_ray_depth = 50;
    //cfg.aa_sample_size = 1;
    //cfg.max_ray_depth = 1;

    raytrace(cfg);

    return 0;
}
