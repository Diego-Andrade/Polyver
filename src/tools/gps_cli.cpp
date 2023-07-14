/**
 * @file gps_cli.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-07-07
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "gps_cli.hpp"

#include <iostream>

#include "../neo6.hpp"
#include "fmt/color.h"
#include "fmt/core.h"
#include "ubx_fmt.hpp"

namespace gps_cli {

    bool is_exit(std::string str) {
        return "q" == str || "quit" == str || "exit" == str;
    }

    void print_help() {
        fmt::print("Valid UBX commands must be one of the following class:\n");
    }

    int single_mode(int argc, char** argv) {
        // Format command: [ClassID] [MessageID] <Payload>
        if (argc >= 3 && strncasecmp("format", argv[0], 6) == 0) {
            auto packet = ubx_fmt::build_packet(argv + 1, argc - 1);
            ubx_fmt::print_packet(packet);

            return 0;
        }

        return -1;
    }

    void command_formatter() {
        fmt::print("\nStarting formatter\n");

        while (true) {
            fmt::print("formatter> ");
            std::string console_input;
            std::getline(std::cin, console_input);

            if (is_exit(console_input)) {
                return;
            }

            auto packet = ubx_fmt::build_packet(console_input);
            if (!packet.empty()) {
                ubx_fmt::print_packet(packet);
            } else {
                fmt::print("Failed to parse message.\n");
            }
        }
    }

    void terminal_read(neo6::neo6_gps& gps) {
        fmt::print("{}\n", gps.read());
    }

    void terminal_send(neo6::neo6_gps& gps) {
        fmt::print("terminal> ");
        std::string console_input;
        std::getline(std::cin, console_input);
        auto packet = ubx_fmt::build_packet(console_input);
        if (!packet.empty()) {
            gps.write(packet.begin(), packet.end());

            // Read response from gps
            for (int i = 0; i < 10; i++)
                fmt::print("{}\n", gps.read());
        } else {
            fmt::print("Failed to parse message\n");
        }
    }

    void command_terminal() {
        fmt::print("\nStarting terminal\n");

        // Opening communicaiton with gps
        neo6::neo6_gps gps("/dev/serial0");

        while (true) {
            fmt::print("terminal> ");
            std::string first_word;
            std::cin >> first_word;

            // Exit terminal
            if (is_exit(first_word)) {
                std::cin.ignore();
                break;
            }

            auto search = TERMINAL_COMMANDS.find(first_word);
            if (search != TERMINAL_COMMANDS.end()) {
                search->second.function(gps);
            } else {
                fmt::print("'{}' not recognized\n", first_word);
            }
        }
    }

    int main(int argc, char** argv) {
        // Single mode
        if (argc > 0) {
            return single_mode(argc, argv);
        }

        // Interactive mode
        fmt::print(fg(fmt::color::sky_blue), "GPS Command Line Interface\n");
        while (true) {
            fmt::print("gps> ");
            std::string console_input;
            std::getline(std::cin, console_input);

            if (is_exit(console_input)) {
                return 0;
            }

            auto search = COMMANDS.find(console_input);
            if (search != COMMANDS.end()) {
                search->second.function();
            } else {
                fmt::print("'{}' command not recognized\n", console_input);
            }
        }

        return 0;
    }
}