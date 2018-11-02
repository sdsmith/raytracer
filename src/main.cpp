#include "cli_arg_parser.h"
#include "image/ppm_image_writer.h"
#include "raytracer.h"
#include "scene/test_scene.h"
#include "type/graphics_types.h"

int main(int argc, char* argv[]) {
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

    if (parse_result.early_exit) { return 0; }

    if (parse_result.print_config) { std::cout << "Config: {" << cfg << "}\n"; }

    // Default to test scene
    if (!cfg.scene) {
        cfg.scene = std::make_unique<Test_Scene>();
    }

    Raytracer raytracer;
    // TODO(sdsmith): image is upside down
    raytracer.start(cfg, std::make_unique<Ppm_Image_Writer>());

    return 0;
}
