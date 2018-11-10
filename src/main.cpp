#include "cli_arg_parser.h"
#include "image/ppm_image_writer.h"
#include "raytracer.h"
#include "scene/test_scene.h"
#include "type/graphics_types.h"

#ifdef _WIN32
#   include <float.h>
#elif defined(linux) || defined(cygwin)
#   include <fenv.h>
#endif

/**
 * \brief Trap on sNaNs produced by floating point. Does not catch qNaNs.
 */
bool trap_fp_faults() {
#ifdef _WIN32
    _clearfp();
    unsigned control_mask = 0;
    return _controlfp_s(&control_mask,
                        _controlfp_s(&control_mask, 0, 0) & ~(_EM_INVALID | _EM_ZERODIVIDE | _EM_OVERFLOW),
                        _MCW_EM) == 0;

#elif defined(linux) || defined(cygwin)
    return feenableexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW) == 0;

#elif defined(TRAP_FP_FAULTS)
#   pragma message ("WARNING: trap_arithmetic_faults: unknown OS, FP trap not supported")
    return false;
#endif
}

int main(int argc, char* argv[]) {
#if !defined(NDEBUG) && defined(TRAP_FP_FAULTS)
    if (!trap_fp_faults()) {
        std::cerr << "Failed to set trap for floating point unit exceptions\n";
    }
#endif

    Config cfg = {};
    Cli_Arg_Parser arg_parser;
    Cli_Arg_Parser::Result parse_result = {};

    cfg.rand_seed = 0;
    cfg.viewport = {1920, 1080};
    cfg.eye = {{3, 3, 2}, {0, 0, -1}};
    cfg.up = {0, 1, 0};
    cfg.vert_fov = 20;
    cfg.aperture = 2.0f;
    cfg.aa_sample_size = 100;
    cfg.max_ray_depth = 50;
    cfg.scene = nullptr;
    cfg.image_file_name = "image.ppm";

    parse_result.config = &cfg;
    if (!arg_parser.parse(parse_result, Cli_Arg_Parser::Args(argc, argv))) {
        return 1;
    }

    // Default to test scene
    if (!cfg.scene) {
        cfg.scene = std::make_unique<Test_Scene>();
    }

    // Print final config prior to raytracing
    if (parse_result.print_config) { std::cout << "Config: {" << cfg << "}\n"; }

    if (parse_result.early_exit) { return 0; }

    Raytracer raytracer;
    raytracer.start(cfg, std::make_unique<Ppm_Image_Writer>());

    return 0;
}
