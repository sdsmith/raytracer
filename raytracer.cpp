#include "vec3.h"
#include "ray.h"

#include <cstdio>

Vec3 lerp(const Vec3& start, const Vec3& end, float t) {
    return (1.0f - t) * start + t * end;
}

Vec3 color(const Ray& r) {
    Vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5f * (unit_direction.y() + 1.0f);
    return lerp(Vec3(1.0f, 1.0f, 1.0f), Vec3(0.5f, 0.7f, 1.0f), t);
}

void gen_ppm() 
{
    int nx = 200;
    int ny = 100;
    printf("P3\n%d %d\n255\n", nx, ny);

    Vec3 lower_left_corner(-2.0f, -1.0f, -1.0f);
    Vec3 horizontal(4.0f, 0.0f, 0.0f);
    Vec3 vertical(0.0f, 2.0f, 0.0f);
    Vec3 origin(0.0f, 0.0f, 0.0f);

    for (int j = ny - 1; j >= 0; --j) {
        for (int i = 0; i < nx; ++i) {
            float u = static_cast<float>(i) / static_cast<float>(nx);
            float v = static_cast<float>(j) / static_cast<float>(ny);
            Ray r(origin, lower_left_corner + u * horizontal + v * vertical);
            
            Vec3 col = color(r);
            int ir = static_cast<int>(255.99 * col.r());
            int ig = static_cast<int>(255.99 * col.g());
            int ib = static_cast<int>(255.99 * col.b());

            printf("%d %d %d\n", ir, ig, ib);
        }
    }
}

int main()
{
    gen_ppm();

    return 0;
}

