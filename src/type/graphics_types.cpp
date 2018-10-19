#include "graphics_types.h"

float Viewport::aspect_ratio() const { return width / static_cast<float>(height); }

//------------------------------------------------------------------------------
RbgFrame::RbgFrame(size_t height, size_t width) {
    pixels.reserve(height);
    for (size_t row = 0; row < height; ++row) {
        pixels.emplace_back(width, Vec3(0.0f, 0.0f, 0.0f));
    }
}
    
RbgFrame::RbgFrame(const Viewport& viewport)
    : RbgFrame(viewport.height, viewport.width) {}

size_t RbgFrame::height() const { return pixels.size(); }

size_t RbgFrame::width() const { return pixels[0].size(); }
