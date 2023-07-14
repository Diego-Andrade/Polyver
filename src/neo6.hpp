#pragma once

/**
 * @file neo6.hpp
 * @author Diego Andrade (bets636@gmail.com)
 * @brief NEO 6 series gps class for sending and receiving messages
 * @version 0.1
 * @date 2023-07-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <array>
#include <memory>
#include <fstream>
#include <cstdint>

namespace neo6
{
    class neo6_gps {
        private:
            static constexpr std::size_t DATA_BUFFER_SIZE = 1000;   // UBX message takes up 5 bytes as '0x00'

            std::fstream data_stream;
            std::array<char, DATA_BUFFER_SIZE> data_buffer;

            

        public:
            neo6_gps(std::string_view stream_path);

            std::string_view read();
            void clear();

            template<typename Iterator>
            void write(const Iterator begin, const Iterator end) {
                for (Iterator it{begin}; it != end; ++it) {
                    data_stream.put(*it);
                }
            }

    };

    /**
     * @brief Reads a single message from GPS
     * 
     * @param os the stream to output message to
     * @param obj the GPS object
     * @return std::ostream& 
     */
    std::ostream& operator<<(std::ostream& os, neo6_gps& obj);

    /**
     * @brief Convert byte to HexString
     * 
     * @param byte to be converted
     * @return std::array<char, 4> with HexString in '0xFF' format
     */
    std::array<char, 4> to_hexstring(const char& byte);

    /**
     * @brief UBX command to enable UBX message for UART port 1
     * 
     */
    static std::array<uint8_t, 20> UART_UBX_EN = {
        0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xc0,
        0x00, 0x00, 0x25, 0x80, 0x00, 0x07, 0x00, 0x03,
        0x00, 0x00, 0x00, 0x00
    };

    /**
     * @brief UBX command to disable GPTXT messages
     * @note Turning off all message for UART 1 and UART 2, except Errors
    */
    static std::array<uint8_t, 14> TXT_OFF = {
        0x06, 0x02, 0x0A, 0x00, 0x01, 0x00, 0x00, 0x00, 
        0x1F, 0x01, 0x01, 0x1F, 0x1F, 0x1F
    };

} // namespace neo6
