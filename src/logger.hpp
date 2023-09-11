/**
 * @file logger.hpp
 * @author Diego Andrade (bets636@gmail.com)
 * @brief Log interface using spdlog
 * @version 0.1
 * @date 2023-09-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <memory>
#include <string_view>

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

class Logger {
   public:
    /** @brief Logger object */
    std::shared_ptr<spdlog::logger> log;

    /** @brief Initialize logger with given name
     * @param name the name of the logger */
    Logger(std::string_view name) {
        std::vector<spdlog::sink_ptr> sinks;
        sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        // sinks.push_back(std::make_shared<spdlog::sinks::daily_file_sink_st>("logfile", 23, 59));
        log = std::make_shared<spdlog::logger>(name.data(), begin(sinks), end(sinks));
    }
};
