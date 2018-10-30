#include "camera.h"
#include "hitable.h"
#include "hitable_list.h"
#include "image/async_image_writer.h"
#include "image/ppm_image_writer.h"
#include "math/ray.h"
#include "math/vec3.h"
#include "scene/random_scene.h"
#include "scene/test_scene.h"
#include "type/graphics_types.h"
#include "utility/strutil.h"
#include "utility/thread_pool.h"
#include "utility/utility.h"

#ifdef _MSC_VER
// Ignore warnings for unsafe std::gmtime
#pragma warning(disable : 4996)
#endif

#include <cassert>
#include <chrono>
#include <cstdlib>
#include <ctime>      // std::gmtime
#include <fstream>
#include <functional>
#include <iomanip>    // std::put_time
#include <limits>
#include <memory>
#include <sstream>
#include <thread>
#include <vector>

class Raytracer
{
// TODO(sdsmith): cleanup this class
public:
    static void async_image_gen(Async_Image_Writer& writer, RbgFrame::Row& row, Config const& cfg, Hitable const& scene, unsigned row_y) {
        color_row(row, cfg, scene, row_y);
        writer.buf_write(Async_Image_Writer::Buf_Entry{row_y, row});
    }
    
    static void start(Config const& cfg, Scene const& scene, Image_Writer& writer) {
        Viewport const& viewport = cfg.viewport;
        assert(viewport.height > 0);
        assert(viewport.width > 0);

        srand(cfg.rand_seed);

        auto const time_point_start = std::chrono::system_clock::now();
        std::time_t const t_start = std::chrono::system_clock::to_time_t(time_point_start);
        std::cout << "Start time: "
                  << std::put_time(std::gmtime(&t_start), "%F %T UTC") << std::endl;
        
        {
            Thread_Pool t_pool;
            RbgFrame frame(cfg.viewport);
            Async_Image_Writer async_writer(std::make_unique<Ppm_Image_Writer>());
            async_writer.open("image.ppm");
            async_writer.write_header(frame);
            
            // Traverse from the viewport's lower left corner to top right corner,
            // left to right, bottom to top.
            for (unsigned row = 0; row < frame.height(); ++row) {
                // Store the image upside down so it displays correctly!
                unsigned const adjusted_row = frame.height() - 1 - row;
                t_pool.add_work(std::bind(&async_image_gen,
                                          std::ref(async_writer),
                                          std::ref(frame.pixels[adjusted_row]),
                                          std::cref(cfg),
                                          std::cref(scene.get()),
                                          row));
            }

            t_pool.enable();

            // TODO: don't spin. Use sentinal.
            while (!t_pool.is_done()) {
                continue;
            }

            async_writer.close();
        }

        auto const time_point_end = std::chrono::system_clock::now();
        std::time_t const t_end = std::chrono::system_clock::to_time_t(time_point_end);
        std::cout << "End time  : "
                  << std::put_time(std::gmtime(&t_end), "%F %T UTC") << std::endl;

        std::cout << "Duration  : "
                  << std::chrono::duration_cast<std::chrono::seconds>(
                      time_point_end - time_point_start).count()
                  << "s" << std::endl;

        // TODO: cleanup memory
    }

private:
    static Vec3 gamma_correction(Vec3 const& col) {
        // Use Gamma 2 (1/gamma => 1/2)
        Vec3 corrCol = Vec3(static_cast<float>(sqrt(col.r())),
                            static_cast<float>(sqrt(col.g())),
                            static_cast<float>(sqrt(col.b()))); // TODO(sdsmith): cast down
        
        // TODO: Getting nans
        // assert(!corrCol.is_nan());
        if (corrCol.is_nan()) {
            corrCol = {0.0f, 0.0f, 0.0f};
        }
        return corrCol;
    }

    static Vec3 color(Ray const& r, Hitable const& scene, unsigned depth, unsigned max_depth) {
        Hit_Record rec;

        if (scene.hit(r, 0.0f + flop_err_thresh, std::numeric_limits<float>::max(), rec))
        {
            assert(rec.material);

            Ray scattered;
            Vec3 attenuation;
            if (depth < max_depth &&
                rec.material->scatter(r, rec, attenuation, scattered)) {
                return attenuation * color(scattered, scene, depth + 1, max_depth);
            } else {
                return Vec3(0.0f, 0.0f, 0.0f);
            }
        } else {
            // Background
            Vec3 const unit_direction = unit_vector(r.direction());
            float const t = 0.5f * (unit_direction.y() + 1.0f); // [-1, 1] -> [0, 1]
            return lerp(Vec3(1.0f, 1.0f, 1.0f), Vec3(0.5f, 0.7f, 1.0f), t);
        }
    }

    static void antialias(Config const& cfg, Camera const& cam, Hitable const& scene,
                   Point2D<unsigned> pixel, Vec3& col)
    {
        for (unsigned s = 0; s < cfg.aa_sample_size; ++s) {
            float const u = (static_cast<float>(pixel.x) + rand_normalized()) / static_cast<float>(cfg.viewport.width);
            float const v = (static_cast<float>(pixel.y) + rand_normalized()) / static_cast<float>(cfg.viewport.height);
            Ray const r = cam.to_viewport(u, v);
            col += color(r, scene, 0, cfg.max_ray_depth);
        }

        col /= static_cast<float>(cfg.aa_sample_size);
        assert(!col.is_nan());
    }

    static void color_pixel(Vec3& pixel, Config const& cfg, Hitable const& scene,
                     Point2D<unsigned> pos)
    {
        float const dist_to_focus = (cfg.eye.origin() - cfg.eye.direction()).length();
        Camera cam(cfg.eye, cfg.up, cfg.vert_fov, cfg.viewport.aspect_ratio(),
                   cfg.aperture, dist_to_focus);

        pixel = {0.0f, 0.0f, 0.0f};

        antialias(cfg, cam, scene, pos, pixel);

        pixel = gamma_correction(pixel);
    }

    static void color_row(RbgFrame::Row& row, Config const& cfg, Hitable const& scene,
                          unsigned pixel_y)
    {
        for (unsigned x = 0; x < cfg.viewport.width; ++x) {
            color_pixel(row[x], cfg, scene, {x, pixel_y});
        }
    }
};

int main() {
    Config cfg = {};
    cfg.rand_seed = 0;
    cfg.viewport = {1920, 1080};
    cfg.eye = {{3, 3, 2}, {0, 0, -1}};
    cfg.up = {0, 1, 0};
    cfg.vert_fov = 20;
    cfg.aperture = 2.0f;
    cfg.aa_sample_size = 100;
    cfg.max_ray_depth = 50;

    Random_Scene scene;
    scene.generate();
    Ppm_Image_Writer writer;
    Raytracer raytracer;
    
    raytracer.start(cfg, scene, writer);

    return 0;
}
