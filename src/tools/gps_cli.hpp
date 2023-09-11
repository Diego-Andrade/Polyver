#pragma once

/**
 * @file gps_cli.hpp
 * @author Diego Andrade (bets636@gmail.com)
 * @brief Simple gps command line interface for sending and receiving messages
 * @version 0.1
 * @date 2023-07-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <functional>
#include <string>
#include <unordered_map>

#include "../neo6.hpp"

namespace gps_cli
{

    template<typename Signature>
    struct command {
        std::string description;
        std::function<Signature> function;
    };

    int main(int argc, char** argv);

    // Modes
    int single_mode(int argc, char** argv);

    // Command
    void command_formatter();
    void command_terminal();

    // Command maps
    const std::unordered_map<std::string, command<void()>> COMMANDS = {
        {"terminal", {"An interactive terminal for NEO6 gps", command_terminal}},
        {"formatter", {"A formatter for hexstrings to ubx messages", command_formatter}}
    };

    // Terminal commands
    void terminal_send(neo6::gps& gps);
    void terminal_read(neo6::gps& gps);

    // Terminal commands map
    const std::unordered_map<std::string, command<void(neo6::gps& gps)>> TERMINAL_COMMANDS = {
        {"send", {"Send the following line encoded as HexString to GPS", terminal_send}},
        {"read", {"Reads a message from GPS", terminal_read}},
        {"r", {"Reads a message from GPS", terminal_read}},
        {"clear", {"Clears GPS messages", [](neo6::gps& gps){ gps.clear(); }}}
    };

    // Helpers
    bool is_exit(std::string str);
    void print_help();
}

