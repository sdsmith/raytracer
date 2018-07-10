#include "camera.h"
#include "hitable.h"
#include "hitable_list.h"
#include "ray.h"
#include "sphere.h"
#include "utility.h"
#include "vec3.h"

#include <cstdio>
#include <cstdlib>
#include <limits>
#include <memory>

Vec3 color(const Ray& r, const Hitable* world);

struct Viewport {
    int width;  /** Pixel width */
    int height; /** Pixel height */
};

float rand_normalized() {
    return rand() / static_cast<float>(RAND_MAX);
}

Vec3 rand_in_unit_sphere() {
    Vec3 p;
    do {
        p = 2.0f * Vec3(rand_normalized(), rand_normalized(), rand_normalized()) - Vec3(1.0f, 1.0f, 1.0f);
    } while (p.squared_length() >= 1.0f);

    return p;
}

Vec3 color_diffuse(const Ray& r, const Hitable* world, const Hit_Record& rec) {
    /*
     * Pick a random point tangent to the hitpoint and shoot another ray. Any 
     * collection of unmodified random rays will derive diffuse light.
     */

    const Vec3& target = rec.p + rec.normal + rand_in_unit_sphere();
    return 0.5f * color(Ray(rec.p, target - rec.p), world); // [-1, 1] -> [0, 1]
}

Vec3 color(const Ray& r, const Hitable* world) {
    Hit_Record rec;

    if (world->hit(r, 0.0f, std::numeric_limits<float>::max(), rec)) {
        return color_diffuse(r, world, rec);
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
        col += color(r, world);
    }

    col /= static_cast<float>(sample_size);
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

    constexpr size_t num_hitables = 2;
    Hitable* list[num_hitables];
    list[0] = new Sphere(Vec3(0.0f, 0.0f, -1.0f), 0.5f);
    list[1] = new Sphere(Vec3(0.0f, -100.5f, -1.0f), 100.0f);
    Hitable* world = new Hitable_List(list, num_hitables);

    constexpr Viewport viewport = { 200, 100 };
    Camera cam;

    gen_ppm(viewport, cam, world);

    return 0;
}

