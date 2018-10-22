#pragma once

#include "math/ray.h"
#include "math/vec3.h"

#include <vector>

struct Viewport {
public:
    unsigned width;  //!< pixel width
    unsigned height; //!< pixel height

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
    std::vector<std::vector<Vec3>> pixels = {}; //!< [row][column]

    RbgFrame(unsigned height, unsigned width);    
    RbgFrame(Viewport const& viewport);
    unsigned height() const;
    unsigned width() const;
};
