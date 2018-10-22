#include "graphics_types.h"

float Viewport::aspect_ratio() const { return static_cast<float>(width) / static_cast<float>(height); }

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
