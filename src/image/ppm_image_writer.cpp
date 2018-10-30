#include "ppm_image_writer.h"

#include <cassert>

void Ppm_Image_Writer::write(Config const& cfg) {
    assert(m_image_file.is_open());
    m_image_file << to_file_format(s_ln_prefix, cfg);
}

void Ppm_Image_Writer::write_header(RbgFrame const& frame) {
    m_image_file << "P3\n"
                 << frame.width() << " " << frame.height() << "\n255\n";                
}

void Ppm_Image_Writer::write(RbgFrame::Row const& row) {
    for (auto const& pixel : row) {
        int ir = static_cast<int>(255.99f * pixel.r());
        int ig = static_cast<int>(255.99f * pixel.g());
        int ib = static_cast<int>(255.99f * pixel.b());

        m_image_file << ir << " " << ig << " " << ib << "\n";
    }
}
