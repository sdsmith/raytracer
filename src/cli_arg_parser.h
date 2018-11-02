#pragma once

#include "type/graphics_types.h"

#include <stdexcept>

class Not_Enough_Cli_Arg_Params : public std::invalid_argument {
public:
    Not_Enough_Cli_Arg_Params() : std::invalid_argument("") {}
};

class Cli_Arg_Parser {
public:
    struct Args {
    public:
        int argc;    //!< argument count
        char** argv; //!< argument values

        Args(int argc, char* argv[]) : argc(argc), argv(argv) {}
    };

    // TODO(sdsmith): doc
    struct Result {
    public:
        Config* config = nullptr;  //!< raytracer configuration
        bool early_exit = false;   //!< exit early, don't raytrace
        bool print_config = false; //!< print raytracer configuration
    };

    /*
     * \brief Parse the given command line args.
     *
     * \param result Result of the parse.
     * \param cli_args Command line args.
     */
    static bool parse(Result& result, Args cli_args) noexcept(false);
};
