#pragma once

/**
 * @file ubx_fmt.hpp
 * @author Diego Andrade (bets636@gmail.com)
 * @brief A formatter and parser for UBX message, a UBLOX
 *        protocol, as described in u-blox 6 GPS/GLONASS/QZSS 
 *        Receiver Description Including Protocol Specification V14
 * @version 0.1
 * @date 2023-07-08
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace ubx_fmt {
    const uint8_t SYNC_CHAR_1 = 0xB5;
    const uint8_t SYNC_CHAR_2 = 0x62;

    using FieldMap = std::unordered_map<std::string, uint8_t>;

    const FieldMap CLASSES = {
        {"NAV", 0x01},
        {"RXM", 0x02},
        {"INF", 0x04},
        {"ACK", 0x05},
        {"CFG", 0x06},
        {"MOV", 0x0A},
        {"AID", 0x0B},
        {"TIM", 0x0D}
    };

    const FieldMap MESSAGES = {
        {"CFG", 0x09},
        {"INF", 0x02},
        {"MSG", 0x01},
        {"NMEA", 0x17},
        {"PM2", 0x3B},
        {"PRT", 0x06}
    };

    std::vector<uint8_t> build_packet(char** params, int count);
    std::vector<uint8_t> build_packet(std::string encoded_command);

    uint8_t parse_field(std::string classID_str);
    uint8_t parse_field(std::string classID_str, const FieldMap& map);

    void print_packet(std::vector<uint8_t> packet);

}