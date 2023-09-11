/**
 * @file neo6.cpp
 * @author Diego Andrade (bets636@gmail.com)
 * @brief Implementation of neo6.hpp
 * @version 0.1
 * @date 2023-07-07
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "neo6.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include "spdlog/spdlog.h"

namespace neo6 {

gps::gps(std::string_view stream_path)
    : gps_fs{stream_path.data(), std::ios::in | std::ios::out},  // std::ios:trunc to discard contents on open
      data_buffer{},
      logger{"NEO6"} {
    if (!gps_fs) {
        logger.log->critical("Failed to open connection at {}", stream_path.data());
        throw std::ios_base::failure("NEO6: Failed to open data stream.");
    }

    logger.log->info("Connection establed at {}/{}", std::filesystem::current_path().string(), stream_path);
}

message gps::read() {
    size_t msg_length{0};

    int last_byte{0};
    while (msg_length < data_buffer.max_size()) {  // TODO: Add limit to prevent hanging, maybe timeout?

        gps_fs.clear();  // Ignore previous stream errors
        int b{gps_fs.get()};

        // NMEA end sequence
        if (b == '\n' && last_byte == '\n' && msg_length > 0) {
            break;
        }

        // NMEA message starting
        if (b == '$' && msg_length > 0) {
            break;
        }

        // Store byte except escape sequence characters
        if (b != '\n' && b != '$') {
            data_buffer[msg_length++] = b;
        }

        last_byte = b;
    }

    message msg;
    msg.NNME = data_buffer[0] == 'G' && data_buffer[1] == 'P';
    msg.data = std::vector<std::uint8_t>(data_buffer.begin(), data_buffer.begin() + msg_length);
    return msg;
}

void gps::clear() {
    gps_fs.seekp(0, std::ios_base::end);
    gps_fs.clear();
}

std::ostream& operator<<(std::ostream& os, gps& gps) {
    auto msg = gps.read();

    if (msg.NNME) {
        for (char b : msg.data) os << b;
    } else {
        for (auto b : msg.data) os << b;
    }

    return os;
}

}  // namespace neo6
