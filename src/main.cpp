/**
 * @file main.cpp
 * @author Diego Andrade (bets636@gmail.com)
 * @brief Main launch point for Polyver program and debugging tools
 * @version 0.1
 * @date 2023-07-07
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "../Debug/version.hpp"
#include "fmt/color.h"
#include "fmt/core.h"
#include "fmt/ranges.h"
#include "neo6.hpp"
#include "tools/gps_cli.hpp"

int polyver_main(int argc, char** argv) {
    // neo6::neo6_gps gps("/dev/serial0");

    neo6::gps gps("../test/gps_sample.txt");
    for (int i = 0; i < 5; ++i) {
        fmt::print("{}\n", gps.read().data);
    }

    return 0;
}

int main(int argc, char** argv) {
    // Handle tool call from cli
    if (argc >= 2 && strncasecmp("gps_cli", argv[1], 11) == 0) {
        return gps_cli::main(argc - 2, (argv + 2));
    }

    fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::green), "\n\nPolyver v{}.{}.{}\n", PROJECT_VERSION_MAJOR,
               PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH);

    return polyver_main(argc, argv);
}
