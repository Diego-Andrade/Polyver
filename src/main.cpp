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

#include "../build/version.hpp"

#include <iostream>

#include "fmt/core.h"
#include "fmt/color.h"
#include "tools/gps_cli.hpp"
#include "neo6.hpp"

int main(int argc, char** argv){

    // Handle tool call from cli
    if (argc >= 2 && strncasecmp("gps_cli", argv[1], 11) == 0) { 
        return gps_cli::main(argc - 2, (argv+2));
    }

    fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::green), "\n\nRelayPi Version {}.{}.{}\n",
        PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH);


    neo6::neo6_gps gps("/dev/serial0");
    for (int i = 0; i < 500; ++i) {
        std::cout << gps << '\n';
    }


    return 0;
}
