#include "Log.h"
#include "Platform.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <vector>

namespace Iridis
{
    std::shared_ptr<spdlog::logger> Logger::s_GlobalLogger;
    std::shared_ptr<spdlog::logger> Logger::s_InternalLogger;
    
    namespace ConsoleColors
    {
#ifdef IRIDIS_WINDOWS
#include <windows.h>
        void Enable()
        {
            HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

            if (outputHandle == INVALID_HANDLE_VALUE)
            {
                return;
                IRIDIS_CORE_ERROR("Could not get the standard output handle.");
            }

            DWORD consoleMode = 0;
            if (!GetConsoleMode(outputHandle, &consoleMode))
            {
                IRIDIS_CORE_ERROR("Could not get the console mode.");
                return;
            }
            
            consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            if (!SetConsoleMode(outputHandle, consoleMode))
            {
                IRIDIS_CORE_ERROR("Could not enable `ENABLE_VIRUAL_TERMINAL_PROCESSING` mode on the output handle.");
                return;
            }
        }
#else
        void Enable() { }
#endif
    }

    std::string WideToNarrow(const std::wstring& wideString)
    {
        std::wostringstream wideOutStringStream;
        wideOutStringStream << wideString;
        std::wstring wstr = wideOutStringStream.str();
        return std::string(wstr.begin(), wstr.end());
    }

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
