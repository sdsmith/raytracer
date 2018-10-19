#pragma once

#include "math/ray.h"
#include "math/vec3.h"

#include <vector>

struct Viewport {
public:
    size_t width;  //!< pixel width
    size_t height; //!< pixel height

    float aspect_ratio() const;
};

struct Config {
public:
    unsigned int rand_seed; //!< seed for randomization
    Viewport viewport;      //!< viewport
    Ray eye;                //!< eye position and look direction
    Vec3 up;                //!< up direction
    float vert_fov;         //!< vertical field of view (in degrees)
    float aperture;         //!< aperture diameter
    int aa_sample_size;     //!< anti-aliasing sample size per pixel
    int max_ray_depth;      //!< max ray iterations per pixel
};

struct RbgFrame {
public:
    std::vector<std::vector<Vec3>> pixels; //!< [row][column]

    RbgFrame(size_t height, size_t width);    
    RbgFrame(const Viewport& viewport);
    size_t height() const;
    size_t width() const;
};
