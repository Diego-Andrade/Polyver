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
#include <fstream>
#include <exception>

namespace neo6 {

    neo6_gps::neo6_gps(std::string_view stream_path) 
    : data_stream{stream_path.data(), std::ios::in | std::ios::out | std::ios::trunc},
      data_buffer{}
    {
        if (!data_stream) {
            throw std::ios_base::failure("Failed to open gps data stream");
        }
    }

    std::string_view neo6_gps::read() {
        int length{0};
        int last_byte{0};
        bool decode_ubx = false;
        while (length < data_buffer.max_size()) {
            data_stream.clear();        // Ignore stream errors
            int b{data_stream.get()};

            // NMEA end sequence
            if (b == '\n' && last_byte == '\n' && length > 0) {
                break;
            }

            // NMEA message starting
            if (b == '$' && length > 0) {
                break;
            }

            // Store byte except escape sequence characters
            if (b != '\n' && b != '$') {
                if (decode_ubx) {
                    for (char c : to_hexstring(b)) {
                        data_buffer[length++] = c;
                    }
                    data_buffer[length++] = ' ';
                } else {
                    data_buffer[length++] = b;
                }
            }

            // Check message type and begin decoding if necessary
            if (length == 2 && !('G' == data_buffer[0] && 'P' == data_buffer[1])) {
                decode_ubx = true;

                // Re-encoding first 2 bytes
                length = 0;
                auto byte1{to_hexstring(data_buffer[0])};
                auto byte2{to_hexstring(data_buffer[1])};

                // Byte 1
                for (char c : byte1) {
                    data_buffer[length++] = c;
                }
                data_buffer[length++] = ' ';

                // Byte 2
                for (char c : byte2) {
                    data_buffer[length++] = c;
                }
                data_buffer[length++] = ' ';

            }

            last_byte = b;
        }

        return {data_buffer.begin(), data_buffer.begin() + length};
    }

    std::ostream& operator<<(std::ostream& os, neo6_gps& obj) {
        os << obj.read();
        return os;
    }

    std::array<char, 4> to_hexstring(const char& byte) {
        std::array<char, 4> decoded{'0', 'x'};

        int upper = byte >> 4;
        decoded[2] = (upper > 9) ? 'A' + upper - 10 : '0' + upper;

        int lower = byte & 0x0f;
        decoded[3] = (lower > 9) ? 'A' + lower -10 : '0' + lower;
        return decoded;
    }

    void neo6_gps::clear() {
        data_stream.seekp(0, std::ios_base::end);
        data_stream.clear();
    }
}

