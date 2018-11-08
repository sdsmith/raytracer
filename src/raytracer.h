#pragma once

#include "camera.h"
#include "hitable.h"
#include "image/async_image_writer.h"
#include "math/ray.h"
#include "math/vec3.h"
#include "scene/scene.h"
#include "type/graphics_types.h"

#include <memory>

class Raytracer
{
// TODO(sdsmith): cleanup this class
public:
    static void start(Config const& cfg, std::unique_ptr<Image_Writer> writer);

private:
    static Vec3 gamma_correction(Vec3 const& col);
    static Vec3 color(Ray const& r, Hitable const& scene, unsigned depth, unsigned max_depth);
    static void antialias(Config const& cfg, Camera const& cam, Hitable const& scene,
                          Point2D<unsigned> const& pixel, Vec3& col);
    static void color_pixel(Vec3& pixel, Config const& cfg, Hitable const& scene,
                            Point2D<unsigned> const& pos);
    static void color_row(RbgFrame::Row& row, Config const& cfg, Hitable const& scene,
                          unsigned image_y);
    static void async_image_gen(Async_Image_Writer& writer, RbgFrame& frame, unsigned row_index, Config const& cfg,
                                Hitable const& scene);
};
