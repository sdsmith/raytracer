#include "cli_arg_parser.h"

#include "scene/random_scene.h"
#include "scene/test_scene.h"
#include "utility/strutil.h"

#include <cassert>
#include <sstream>

using Str_Util::string_to_unsigned;

bool Cli_Arg_Parser::parse(Result& result, Args cli_args) noexcept(false) {
    assert(result.config);

    Config& cfg = *result.config;
    bool is_parse_error = false;
    result.early_exit = false;
    result.print_config = false;

    std::stringstream help_text;
    help_text << "usage: " << cli_args.argv[0] << " [args]\n"
              << "argument            parameter(s)            description\n"
              << "-h --help                                   help\n"
              << "--viewport          <u:width> <u:height>    viewport size in pixels\n"
              << "--rand_seed         <u:seed>                randomization seed\n"
              << "--vert_fov          <f:degree>              vertical field of view in degrees\n"
              << "--aperture          <f:diameter>            camera aperture diameter\n"
              << "--aa_sample_size    <u>                     antialiasing sample size\n"
              << "--max_ray_depth     <u>                     maximum ray depth\n"
              << "-s --scene          <s:name>                select scene {test,random}\n"
              << "--print_config                              print raytracer configuration\n"
              << "-o --output_file    <s:filename>            output image file\n"
              << "--exit                                      exit prior to raytracing\n";
    //<< "--load_scene        <filename>              \n"
    //<< "--load_config       <filename>              \n";

    // TODO(sdsmith): support eye and up
    for (int i = 1; i < cli_args.argc; ++i) {
        std::string const arg = cli_args.argv[i];

        try {
            if (arg == "-h" || arg == "--help") {
                std::cout << help_text.rdbuf();
                result.early_exit = true;

            } else if (arg == "--rand_seed") {
                if (i + 1 > cli_args.argc) { throw Not_Enough_Cli_Arg_Params(); }
                cfg.rand_seed = string_to_unsigned(cli_args.argv[++i]);

            } else if (arg == "--viewport") {
                if (i + 2 > cli_args.argc) { throw Not_Enough_Cli_Arg_Params(); }
                cfg.viewport = { string_to_unsigned(cli_args.argv[++i]),
                                 string_to_unsigned(cli_args.argv[++i]) };

            } else if (arg == "--vert_fov") {
                if (i + 1 > cli_args.argc) { throw Not_Enough_Cli_Arg_Params(); }
                cfg.vert_fov = std::stof(cli_args.argv[++i]);

            } else if (arg == "--aperture") {
                if (i + 1 > cli_args.argc) { throw Not_Enough_Cli_Arg_Params(); }
                cfg.aperture = std::stof(cli_args.argv[++i]);

            } else if (arg == "--aa_sample_size") {
                if (i + 1 > cli_args.argc) { throw Not_Enough_Cli_Arg_Params(); }
                cfg.aa_sample_size = string_to_unsigned(cli_args.argv[++i]);

            } else if (arg == "--max_ray_depth") {
                if (i + 1 > cli_args.argc) { throw Not_Enough_Cli_Arg_Params(); }
                cfg.max_ray_depth = string_to_unsigned(cli_args.argv[++i]);

            } else if (arg == "-s" || arg == "--scene") {
                if (i + 1 > cli_args.argc) { throw Not_Enough_Cli_Arg_Params(); }

                std::string const scene_name = cli_args.argv[++i];
                if (scene_name == "test") {
                    cfg.scene = std::make_unique<Test_Scene>();
                } else if (scene_name == "random") {
                    cfg.scene = std::make_unique<Random_Scene>();
                } else {
                    std::cerr << arg << ": unknown scene: " << scene_name << "\n";
                    is_parse_error = true;
                }

            } else if (arg == "--print_config") {
                result.print_config = true;

            } else if (arg == "-o" || arg == "--output_file") {
                if (i + 1 > cli_args.argc) { throw Not_Enough_Cli_Arg_Params(); }
                cfg.image_file_name = cli_args.argv[++i];

            } else if (arg == "--exit") {
                result.early_exit = true;

            } else {
                std::cerr << "unknown argument [" << i - 1 << "]: " << arg << "\n";
                is_parse_error = true;
            }

            // Process next arg
            continue;
        } catch (Not_Enough_Cli_Arg_Params const&) {
            std::cerr << arg << ": invalid arg: " << arg << ": not enough parameters\n";
            is_parse_error = true;
        } catch (std::exception const& e) {
            std::cerr << arg << ": invalid arg: " << arg << ": " << e.what() << "\n";
            is_parse_error = true;
        }

        // Bad parse, return error
        return false;
    }

    if (is_parse_error) {
        return false;
    }

    return true;
}
