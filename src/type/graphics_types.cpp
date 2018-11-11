#include "graphics_types.h"

float Viewport::aspect_ratio() const { return static_cast<float>(width) / static_cast<float>(height); }

std::ostream& operator<<(std::ostream& os, Viewport const& viewport)
{
    return os << viewport.width << " " << viewport.height;
}

//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, Config const& cfg) {
    return os << "scene=" << cfg.scene->name()
              << ", rand_seed=" << cfg.rand_seed
              << ", viewport={" << cfg.viewport << "}"
              << ", eye={" << cfg.eye << "}"
              << ", up={" << cfg.up << "}"
              << ", vert_fov=" << cfg.vert_fov
              << ", aperture=" << cfg.aperture
              << ", aa_sample_size=" << cfg.aa_sample_size
              << ", max_ray_depth=" << cfg.max_ray_depth
              << ", num_threads=" << cfg.num_threads;
}

//------------------------------------------------------------------------------
RbgFrame::RbgFrame(unsigned height, unsigned width) {
    pixels.reserve(height);
    for (unsigned row = 0; row < height; ++row) {
        pixels.emplace_back(width, Vec3(0.0f, 0.0f, 0.0f));
    }
}

RbgFrame::RbgFrame(Viewport const& viewport)
    : RbgFrame(viewport.height, viewport.width) {}

unsigned RbgFrame::height() const { return static_cast<unsigned>(pixels.size()); /*TODO(sdsmith): cast down*/}

unsigned RbgFrame::width() const { return static_cast<unsigned>(pixels[0].size()); /*TODO(sdsmith): cast down*/}
