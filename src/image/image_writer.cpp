#include "image_writer.h"

#include "utility/strutil.h"

void Image_Writer::open(std::string const& filename) {
    m_image_file.open(filename, std::ios::out | std::ios::trunc);
    // TODO(sdsmith): check return
}

void Image_Writer::close() {
    m_image_file.close();
    // TODO(sdsmith): check return
}
    
std::string Image_Writer::to_file_format(char const* ln_prefix, Config const& cfg) const {
    return Str_Util::format(
        "%s Seed            : %X\n"
        "%s Window          : %zux%zu\n"
        "%s Camera          : origin(%s), fwd(%s), up(%s)\n"
        "%s FOV (vert)      : %f\n"
        "%s Aperture        : %f\n"
        "%s AA sample/px    : %d\n"
        "%s Ray depth (max) : %d\n",
        ln_prefix, cfg.rand_seed, ln_prefix, cfg.viewport.width,
        cfg.viewport.height, ln_prefix, cfg.eye.origin().to_string().c_str(),
        cfg.eye.direction().to_string().c_str(), cfg.up.to_string().c_str(),
        ln_prefix, cfg.vert_fov, ln_prefix, cfg.aperture, ln_prefix,
        cfg.aa_sample_size, ln_prefix, cfg.max_ray_depth);
}
