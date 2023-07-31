#pragma once

#define SPDLOG_LEVEL_NAMES { "TRACE",   "DEBUG", "INFO", "WARNING", \
                             "ERROR", "CRITICAL", "OFF" }

#include <spdlog/spdlog.h>
#include <memory>

namespace Iridis
{
    namespace ConsoleColors
    {
        constexpr const char* RESET   = "\033[0m";
        constexpr const char* BOLD    = "\033[1m";
        constexpr const char* ITALIC  = "\033[3m";
        constexpr const char* RED     = "\033[91m";
        constexpr const char* GREEN   = "\033[92m";
        constexpr const char* YELLOW  = "\033[93m";
        constexpr const char* BLUE    = "\033[94m";
        constexpr const char* MAGENTA = "\033[95m";
        constexpr const char* CYAN    = "\033[96m";

        void Enable();
    };

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
#define IRIDIS_DEBUG(...)  ::Iridis::Logger::GetGlobalLogger()->debug(__VA_ARGS__)

#define IRIDIS_CORE_ERROR(...) ::Iridis::Logger::GetInternalLogger()->error(__VA_ARGS__) 
#define IRIDIS_CORE_WARN(...)  ::Iridis::Logger::GetInternalLogger()->warn(__VA_ARGS__) 
#define IRIDIS_CORE_INFO(...)  ::Iridis::Logger::GetInternalLogger()->info(__VA_ARGS__) 
