#include "hitable.h"
#include "hitable_list.h"
#include "ray.h"
#include "sphere.h"
#include "utility.h"
#include "vec3.h"

#include <cstdio>
#include <limits>
#include <memory>

Vec3 color(const Ray& r, Hitable* world) {
    Hit_Record rec;

    if (world->hit(r, 0.0f, std::numeric_limits<float>::max(), rec)) {
        const Vec3& N = rec.normal;
        return 0.5f * Vec3(N.x() + 1, N.y() + 1, N.z() + 1); // [-1, 1] -> [0, 1]
    } else {
        // Background
        Vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5f * (unit_direction.y() + 1.0f); // [-1, 1] -> [0, 1]
        return lerp(Vec3(1.0f, 1.0f, 1.0f), Vec3(0.5f, 0.7f, 1.0f), t);
    }
}

void gen_ppm(Hitable* world)
{
    int nx = 200;
    int ny = 100;
    printf("P3\n%d %d\n255\n", nx, ny);

    Vec3 lower_left_corner(-2.0f, -1.0f, -1.0f);
    Vec3 horizontal(4.0f, 0.0f, 0.0f);
    Vec3 vertical(0.0f, 2.0f, 0.0f);
    Vec3 origin(0.0f, 0.0f, 0.0f);

    // Traverse from the viewport's lower left corner to top right corner, 
    // left to right, bottom to top.
    for (int j = ny - 1; j >= 0; --j) {
        for (int i = 0; i < nx; ++i) {
            float u = static_cast<float>(i) / static_cast<float>(nx);
            float v = static_cast<float>(j) / static_cast<float>(ny);
            Ray r(origin, lower_left_corner + u * horizontal + v * vertical);
            
            Vec3 col = color(r, world);
            int ir = static_cast<int>(255.99 * col.r());
            int ig = static_cast<int>(255.99 * col.g());
            int ib = static_cast<int>(255.99 * col.b());

            printf("%d %d %d\n", ir, ig, ib);
        }
    }
}

int main()
{
    constexpr size_t num_hitables = 2;
    Hitable* list[num_hitables];
    list[0] = new Sphere(Vec3(0.0f, 0.0f, -1.0f), 0.5f);
    list[1] = new Sphere(Vec3(0.0f, -100.5f, -1.0f), 100.0f);
    Hitable* world = new Hitable_List(list, num_hitables);

    gen_ppm(world);

    return 0;
}

