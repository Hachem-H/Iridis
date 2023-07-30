#pragma once

#define SPDLOG_LEVEL_NAMES { "TRACE",   "DEBUG", "INFO", "WARNING", \
                             "ERROR", "CRITICAL", "OFF" }

#include <spdlog/spdlog.h>
#include <memory>

namespace Iridis
{
    class Logger
    {
    public:
        static void Init();

        static std::shared_ptr<spdlog::logger>& GetGlobalLogger()   { return s_GlobalLogger;   }
        static std::shared_ptr<spdlog::logger>& GetInternalLogger() { return s_InternalLogger; }
    private:
        static std::shared_ptr<spdlog::logger> s_GlobalLogger;
        static std::shared_ptr<spdlog::logger> s_InternalLogger;
    };
};

#define IRIDIS_ERROR(...) ::Iridis::Logger::GetGlobalLogger()->error(__VA_ARGS__)
#define IRIDIS_WARN(...)  ::Iridis::Logger::GetGlobalLogger()->warn(__VA_ARGS__)
#define IRIDIS_INFO(...)  ::Iridis::Logger::GetGlobalLogger()->info(__VA_ARGS__)

#define IRIDIS_CORE_ERROR(...) ::Iridis::Logger::GetInternalLogger()->error(__VA_ARGS__) 
#define IRIDIS_CORE_WARN(...)  ::Iridis::Logger::GetInternalLogger()->warn(__VA_ARGS__) 
#define IRIDIS_CORE_INFO(...)  ::Iridis::Logger::GetInternalLogger()->info(__VA_ARGS__) 
