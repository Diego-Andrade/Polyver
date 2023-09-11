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

// TOOD: Insert fmt formatter for GPS objects
//       Make list of UBX commands to better contain


#include <array>
#include <concepts>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <memory>
#include <vector>

#include "logger.hpp"

namespace neo6 {

template <typename Iterator>
concept Uint8Iterator = std::unsigned_integral<typename std::iterator_traits<Iterator>::value_type>;

typedef struct message {
    bool NNME = true;
    std::vector<std::uint8_t> data;
} message;

class gps {
   private:
    static constexpr std::size_t DATA_BUFFER_SIZE = 100;

    /** @brief The sensor data stream */
    std::fstream gps_fs;

    /** @brief Buffer for incomplete messages */
    std::array<std::uint8_t, DATA_BUFFER_SIZE> data_buffer;

    /** @brief NEO6 event logger */
    Logger logger;

   public:
    /**
     * @brief Initialize NEO6-based gps module
     * @param stream_path the file path to gps data stream */
    gps(std::string_view stream_path);

    /** @brief Read a single MNEA message */
    message read();

    /**
     * @brief Send data
     * @param begin iterator of data
     * @param end iterator of data */
    template <Uint8Iterator Iterator>
    void write(const Iterator begin, const Iterator end) {
        for (Iterator it{begin}; it != end; ++it) {
            gps_fs.put(*it);
        }
    }

    /** @brief Clear message queue */
    void clear();
};

/**
 * @brief Reads a single message from GPS
 * @param os the stream to output message to
 * @param obj the GPS object
 * @return std::ostream&
 */
std::ostream& operator<<(std::ostream& os, gps& obj);

/**
 * @brief UBX command to enable UBX message for UART port 1
 *
 */
static std::array<uint8_t, 20> UART_UBX_EN = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xc0, 0x00, 0x00,
                                              0x25, 0x80, 0x00, 0x07, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00};

/**
 * @brief UBX command to disable GPTXT messages
 * @note Turning off all message for UART 1 and UART 2, except Errors
 */
static std::array<uint8_t, 14> TXT_OFF = {0x06, 0x02, 0x0A, 0x00, 0x01, 0x00, 0x00,
                                          0x00, 0x1F, 0x01, 0x01, 0x1F, 0x1F, 0x1F};

}  // namespace neo6
