#pragma once

#include "math/ray.h"
#include "math/vec3.h"
#include "scene/scene.h"

#include <memory>
#include <ostream>
#include <vector>

struct Viewport {
public:
    unsigned width;  //!< pixel width
    unsigned height; //!< pixel height

    float aspect_ratio() const;
};

std::ostream& operator<<(std::ostream& os, Viewport const& viewport);

// TODO(sdsmith): move to Raytracer
struct Config {
public:
    unsigned rand_seed;      //!< seed for randomization
    Viewport viewport;       //!< viewport
    Ray eye;                 //!< eye position and look direction
    Vec3 up;                 //!< up direction
    float vert_fov;          //!< vertical field of view (in degrees)
    float aperture;          //!< aperture diameter
    float dist_to_focus;     //!< camera focus distance
    unsigned aa_sample_size; //!< anti-aliasing sample size per pixel
    unsigned max_ray_depth;  //!< max ray iterations per pixel
    std::unique_ptr<Scene> scene; //!< scene to raytrace
    std::string image_file_name; //!< output image file name
    unsigned num_threads;    //!< number of threads to raytrace with
    Time_Interval shutter_interval; //!< time interval for the camera shutter
};

std::ostream& operator<<(std::ostream& os, Config const& cfg);

struct Rbg_Frame {
public:
    using Row = std::vector<Vec3>;
    std::vector<Row> pixels = {}; //!< [row][column]

    Rbg_Frame(unsigned height, unsigned width);
    Rbg_Frame(Viewport const& viewport);
    unsigned height() const;
    unsigned width() const;
};
