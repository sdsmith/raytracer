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

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <memory>
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
    printf("P3\n%d %d\n255\n", viewport.width, viewport.height);

    // Traverse from the viewport's lower left corner to top right corner, 
    // left to right, bottom to top.
    for (int j = viewport.height - 1; j >= 0; --j) {
        for (int i = 0; i < viewport.width; ++i) {
            Vec3 col(0.0f, 0.0f, 0.0f);

            antialias(viewport, cam, world, i, j, col);
            assert(!is_nan(col));

            col = gamma_correction(col);
            assert(!is_nan(col));

            int ir = static_cast<int>(255.99 * col.r());
            int ig = static_cast<int>(255.99 * col.g());
            int ib = static_cast<int>(255.99 * col.b());

            printf("%d %d %d\n", ir, ig, ib);
        }
    }
}

int main()
{
    std::vector<Hitable*> hitables;

    srand(0);

    hitables.push_back(new Sphere({0.0f, -100.5f, -1.0f}, 100.0f, new Lambertian({0.8f, 0.8f, 0.0f}))); // base
    { // glass sphere
        hitables.push_back(new Sphere({-1.0f, 0.0f, -1.0f}, 0.5f, new Dielectric(1.5f)));
        hitables.push_back(new Sphere({-1.0f, 0.0f, -1.0f}, -0.45f, new Dielectric(1.5f)));
    }
    hitables.push_back(new Sphere({0.0f, 0.0f, -1.0f}, 0.5f, new Lambertian({0.1f, 0.2f, 0.5f})));
    hitables.push_back(new Sphere({1.0f, 0.0f, -1.0f}, 0.5f, new Metal({0.8f, 0.6f, 0.2f}, 1.0f)));

    std::unique_ptr<Hitable> world = std::make_unique<Hitable_List>(hitables.data(), hitables.size());

    constexpr Viewport viewport = {200, 100};
    Camera cam(Ray({-2, 2,  1}/*origin*/, 
                   { 0, 0, -1}/*fwd*/), 
                   { 0, 1,  0}/*up*/, 
                   90/*degree vertFov*/, 
                   viewport.aspect_ratio());

    gen_ppm(viewport, cam, world.get());

    // TODO: cleanup memory

    return 0;
}

