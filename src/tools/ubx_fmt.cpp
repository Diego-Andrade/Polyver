/**
 * @file ubx_fmt.cpp
 * @author Diego Andrade (bets636@gmail.com)
 * @brief Implementaion of ubx_fmt.hpp
 * @version 0.1
 * @date 2023-07-08
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "ubx_fmt.hpp"

#include "fmt/core.h"

#include <cstdio>
#include <sstream>

uint8_t ubx_fmt::parse_field(std::string str) {
    if (str.substr(0, 2) == "0x" && str.length() == 4 &&
              ((str[2] >= 'A' && str[2] <= 'F') || (str[2] >= 'a' && str[2] <= 'f') || (str[2] >= '0' && str[2] <= '9')) &&
              ((str[3] >= 'A' && str[3] <= 'F') || (str[3] >= 'a' && str[3] <= 'f') || (str[3] >= '0' && str[3] <= '9'))
    ) {
        return (uint8_t)strtoul(str.c_str(), NULL, 0);
    }

    return 0xFF;
}

uint8_t ubx_fmt::parse_field(std::string str, const FieldMap& map) {
   if (str.substr(0, 2) == "0x" && str.length() == 4 &&
              ((str[2] >= 'A' && str[2] <= 'F') || (str[2] >= 'a' && str[2] <= 'f') || (str[2] >= '0' && str[2] <= '9')) &&
              ((str[3] >= 'A' && str[3] <= 'F') || (str[3] >= 'a' && str[3] <= 'f') || (str[3] >= '0' && str[3] <= '9'))
    ) {
        return (uint8_t)strtoul(str.c_str(), NULL, 0);
    } else {
        auto search = map.find(str);
        if (search == map.end()) {
            return 0xFF;
        }

        return search->second;
    }
}


std::vector<uint8_t> ubx_fmt::build_packet(std::string encoded_command) {
    std::istringstream command{encoded_command};

    // Exract Class ID
    std::string classID_str;
    command >> classID_str;
    uint8_t classID{parse_field(classID_str, CLASSES)};
    if (classID == 0xFF) {
        fmt::print(stderr, "ClassID '{}' not valid.\n", classID_str);
        return {};
    }

    // Extract Message ID
    std::string messageID_str;
    command >> messageID_str;
    uint8_t messageID{parse_field(messageID_str, MESSAGES)};
    if (messageID == 0xFF) {
        fmt::print(stderr, "MessageID '{}' not valid.\n", messageID_str);
        return {};
    }

    // Build packet and compute checksum
    std::vector<uint8_t> packet = { SYNC_CHAR_1, SYNC_CHAR_2, classID, messageID };

    uint8_t checksum_A = 0, checksum_B = 0;
    checksum_A = classID + messageID;
    checksum_B = classID + classID + messageID;

    std::string payload_str;
    while (command >> payload_str) {
        uint8_t payload{parse_field(payload_str)};
        packet.push_back(payload);
        checksum_A = checksum_A + payload;
        checksum_B = checksum_B + checksum_A;
    }

    packet.push_back(checksum_A);
    packet.push_back(checksum_B);

    return packet;
}

std::vector<uint8_t> ubx_fmt::build_packet(char** params, int count) {
    // Exract Class ID
    uint8_t classID{parse_field(params[0], CLASSES)};

    // Extract Message ID
    uint8_t messageID{parse_field(params[1], MESSAGES)};

    // Build packet and compute checksum
    std::vector<uint8_t> packet = { SYNC_CHAR_1, SYNC_CHAR_2, classID, messageID };

    uint8_t checksum_A = 0, checksum_B = 0;
    checksum_A = classID + messageID;
    checksum_B = classID + classID + messageID;

    for (int i = 2; i < count; ++i) {
        uint8_t payload{parse_field(params[i])};
        packet.push_back(payload);
        checksum_A = checksum_A + payload;
        checksum_B = checksum_B + checksum_A;
    }

    packet.push_back(checksum_A);
    packet.push_back(checksum_B);

    return packet;
}

void ubx_fmt::print_packet(std::vector<uint8_t> packet) {
    if (packet.empty()) {
        fmt::print("<Empty>\n");
        return;
    }
    
    for (uint8_t b : packet) {
        fmt::print("{:#04x} ", b);
    }
    fmt::print("\n");
}