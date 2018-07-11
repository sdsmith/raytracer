#include "camera.h"
#include "hitable.h"
#include "hitable_list.h"
#include "lambertian.h"
#include "metal.h"
#include "ray.h"
#include "sphere.h"
#include "utility.h"
#include "vec3.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <memory>
#include <vector>

struct Viewport {
    int width;  /** Pixel width */
    int height; /** Pixel height */
};

Vec3 color(const Ray& r, const Hitable* world, int depth) {
    Hit_Record rec;

    constexpr float error_margin = 0.001f;
    constexpr int max_depth = 50;

    if (world->hit(r, 0.0f + error_margin, std::numeric_limits<float>::max(), rec)) {
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

void antialias(const Viewport& viewport, const Camera& cam, const Hitable* world, int pix_u, int pix_v, Vec3& col, int depth) {
    constexpr int sample_size = 100;
    
    for (int s = 0; s < sample_size; ++s) {
        float u = static_cast<float>(pix_u + rand_normalized()) / static_cast<float>(viewport.width);
        float v = static_cast<float>(pix_v + rand_normalized()) / static_cast<float>(viewport.height);
        Ray r = cam.to_viewport(u, v);
        Vec3 p = r.point(2.0f); // TODO: Should this be a constant?
        col += color(r, world, depth);
    }

    col /= static_cast<float>(sample_size);
}

Vec3 gamma_correction(const Vec3& col) {
    // Use Gamma 2 (1/gamma => 1/2)

    return Vec3(sqrt(col.r()), sqrt(col.g()), sqrt(col.b()));
}

void gen_ppm(const Viewport& viewport, const Camera& cam, const Hitable* world)
{
    printf("P3\n%d %d\n255\n", viewport.width, viewport.height);

    // Traverse from the viewport's lower left corner to top right corner, 
    // left to right, bottom to top.
    for (int j = viewport.height - 1; j >= 0; --j) {
        for (int i = 0; i < viewport.width; ++i) {
            Vec3 col(0.0f, 0.0f, 0.0f);
            
            antialias(viewport, cam, world, i, j, col, 0);

            col = gamma_correction(col);
            int ir = static_cast<int>(255.99 * col.r());
            int ig = static_cast<int>(255.99 * col.g());
            int ib = static_cast<int>(255.99 * col.b());

            printf("%d %d %d\n", ir, ig, ib);
        }
    }
}

int main()
{
    srand(0);

    std::vector<Hitable*> hitables;
    hitables.push_back(new Sphere({ 0.0f, -100.5f, -1.0f }, 100.0f, new Lambertian({ 0.8f, 0.8f, 0.0f }))); // base
    hitables.push_back(new Sphere({ -1.0f, 0.0f, -1.0f }, 0.5f, new Metal({ 0.8f, 0.8f, 0.8f }, 0.3f)));
    hitables.push_back(new Sphere({ 0.0f, 0.0f, -1.0f }, 0.5f, new Lambertian({ 0.8f, 0.3f, 0.3f })));
    hitables.push_back(new Sphere({ 1.0f, 0.0f, -1.0f }, 0.5f, new Metal({ 0.8f, 0.6f, 0.2f }, 1.0f)));
    std::unique_ptr<Hitable> world = std::make_unique<Hitable_List>(hitables.data(), hitables.size());

    constexpr Viewport viewport = { 200, 100 };
    Camera cam;

    gen_ppm(viewport, cam, world.get());

    // TODO: cleanup memory

    return 0;
}

