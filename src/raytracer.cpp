#include "raytracer.h"

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
#include <ostream>

void Raytracer::start(Config const& cfg, std::unique_ptr<Image_Writer> writer) {
    // Set random seed first
    srand(cfg.rand_seed);

    Viewport const& viewport = cfg.viewport;
    assert(viewport.height > 0);
    assert(viewport.width > 0);

    cfg.scene->generate();

    auto const time_point_start = std::chrono::system_clock::now();
    std::time_t const t_start = std::chrono::system_clock::to_time_t(time_point_start);
    std::cout << "Start time: "
              << std::put_time(std::gmtime(&t_start), "%F %T UTC") << std::endl;

    {
        Thread_Pool t_pool(cfg.num_threads);
        Rbg_Frame frame(viewport);
        Async_Image_Writer async_writer(std::move(writer));
        async_writer.open(cfg.image_file_name);
        async_writer.write_header(frame);

        for (unsigned row = 0; row < frame.height(); ++row) {
            t_pool.add_work(std::bind(&async_image_gen,
                                      std::ref(async_writer),
                                      std::ref(frame),
                                      row,
                                      std::cref(cfg)));
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

    std::cout << "Image written to " << cfg.image_file_name << "\n";

    std::cout << std::endl; // Flush

    // TODO: cleanup memory
}

Vec3 Raytracer::gamma_correction(Vec3 const& col) {
    // Use Gamma 2 (1/gamma => 1/2)
    Vec3 corrCol = Vec3(static_cast<float>(sqrt(col.r())),
                        static_cast<float>(sqrt(col.g())),
                        static_cast<float>(sqrt(col.b()))); // TODO(sdsmith): cast down

    assert(!corrCol.is_nan());
    return corrCol;
}

Vec3 Raytracer::color(Ray const& r, Hitable const& scene, unsigned depth, unsigned max_depth) {
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

void Raytracer::antialias(Config const& cfg, Camera const& cam, Point2D<unsigned> const& pixel, Vec3& col)
{
    for (unsigned s = 0; s < cfg.aa_sample_size; ++s) {
        float const u = (static_cast<float>(pixel.x) + rand_normalized()) / static_cast<float>(cfg.viewport.width);
        float const v = (static_cast<float>(pixel.y) + rand_normalized()) / static_cast<float>(cfg.viewport.height);
        Ray const r = cam.to_viewport(u, v);
        col += color(r, cfg.scene->get(), 0, cfg.max_ray_depth);
    }

    col /= static_cast<float>(cfg.aa_sample_size);
    assert(!col.is_nan());
}

void Raytracer::color_pixel(Vec3& pixel, Config const& cfg, Point2D<unsigned> const& pos)
{
    Camera cam(cfg.eye, cfg.up, cfg.vert_fov, cfg.viewport.aspect_ratio(),
               cfg.aperture, cfg.dist_to_focus, cfg.shutter_interval);

    pixel = {0.0f, 0.0f, 0.0f};

    antialias(cfg, cam, pos, pixel);

    pixel = gamma_correction(pixel);
}

void Raytracer::color_row(Rbg_Frame::Row& row, Config const& cfg, unsigned image_y)
{
    for (unsigned x = 0; x < cfg.viewport.width; ++x) {
        color_pixel(row[x], cfg, {x, image_y});
    }
}

void Raytracer::async_image_gen(Async_Image_Writer& writer, Rbg_Frame& frame, unsigned row_index, Config const& cfg)
{
    /*
     * When we write the image, we want to write the top most row first. The row
     * index starts at 0 at the top most row and increases downward. In image
     * coordinates, the top most row corresponds to the height y value.
     */
    unsigned const image_y = frame.height() - 1 - row_index;
    Rbg_Frame::Row& row = frame.pixels[row_index];

    color_row(row, cfg, image_y);
    writer.buf_write(Async_Image_Writer::Buf_Entry{row_index, row});
}
