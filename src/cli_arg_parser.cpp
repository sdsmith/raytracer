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
              << "--exit                                      exit prior to raytracing\n"
              << "--eye               <f:x0> <f:y0> <f:z0>, <f:x1> <f:y1> <f:z1> camera eye position\n"
              << "--up                <f:x> <f:y> <f:z>       eye up direction\n"
              << "--num_threads       <u>                     number of threads to run while raytracing\n"
              << "--shutter           <f:start> <f:end>       camera shutter interval\n";

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

            } else if (arg == "--eye") {
                constexpr int num_expected_args = 6;
                if (i + num_expected_args > cli_args.argc) { throw Not_Enough_Cli_Arg_Params(); }

                Vec3 v;
                for (int arg_num = 0; arg_num < num_expected_args; ++arg_num) {
                    std::string arg(cli_args.argv[++i]);
                    size_t comma_pos = arg.find(",");
                    if (comma_pos != std::string::npos) {
                        if (arg.size() == 1) {
                            // Comma is a seperate arg (ie. ","), move to the next arg
                            arg = cli_args.argv[++i];
                        } else if (arg_num == 2 || arg_num == 3) {
                            // Comma is at the start or end of a value (ie. "1,", ",1"), strip off the comma
                            arg.erase(comma_pos, 1);
                        } else {
                            std::cerr << arg << ": invalid location for arg seperator ','\n";
                            is_parse_error = true;
                            break;
                        }
                    }

                    v.e[arg_num % Vec3::dimension] = std::stof(arg);

                    switch (arg_num) {
                        case 2: { cfg.eye.p1 = v; v = {0,0,0}; } break;
                        case 5: { cfg.eye.p2 = v; } break;
                        default: break;
                    }
                }

            } else if (arg == "--up") {
                if (i + 3 > cli_args.argc) { throw Not_Enough_Cli_Arg_Params(); }
                for (int arg_num = 0; arg_num < Vec3::dimension; ++arg_num) {
                    cfg.up.e[static_cast<size_t>(arg_num)] = std::stof(cli_args.argv[++i]);
                }

            } else if (arg == "--num_threads") {
                if (i + 1 > cli_args.argc) { throw Not_Enough_Cli_Arg_Params(); }
                cfg.num_threads = string_to_unsigned(cli_args.argv[++i]);

            } else if (arg == "--shutter") {
                if (i + 2 > cli_args.argc) { throw Not_Enough_Cli_Arg_Params(); }
                cfg.shutter_interval = { std::stof(cli_args.argv[++i]),
                                         std::stof(cli_args.argv[++i]) };

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

    return !is_parse_error;
}
