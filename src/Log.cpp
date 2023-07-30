#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

#include <vector>

namespace Iridis
{
    std::shared_ptr<spdlog::logger> Logger::s_GlobalLogger;
    std::shared_ptr<spdlog::logger> Logger::s_InternalLogger;
    
    void Logger::Init()
    {
		std::vector<spdlog::sink_ptr> globalLogSinks;
		globalLogSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        globalLogSinks[0]->set_pattern("[%^%l%$]: %v");

		std::vector<spdlog::sink_ptr> internalLogSinks;
		internalLogSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        internalLogSinks[0]->set_pattern("[%^IRIDIS %l%$]: %v");

        s_GlobalLogger   = std::make_shared<spdlog::logger>("GLOBAL", begin(globalLogSinks), end(globalLogSinks));
        s_InternalLogger = std::make_shared<spdlog::logger>("INTERNAL", begin(internalLogSinks), end(internalLogSinks));

        spdlog::register_logger(s_GlobalLogger);
        spdlog::register_logger(s_InternalLogger);
    }
};
