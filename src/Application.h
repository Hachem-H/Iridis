#pragma once

#include <string>
#include <optional>

#define PrintError std::cout << Iridis::TerminalColors::BOLD << Iridis::TerminalColors::RED << "ERROR: " << Iridis::TerminalColors::RESET

namespace Iridis
{
    namespace TerminalColors
    {
#ifdef _WIN32
    constexpr const char* RESET   = "";
    constexpr const char* BOLD    = "";
    constexpr const char* RED     = "";
    constexpr const char* GREEN   = "";
    constexpr const char* YELLOW  = "";
    constexpr const char* BLUE    = "";
    constexpr const char* MAGENTA = "";
    constexpr const char* CYAN    = "";
#else
    constexpr const char* RESET   = "\033[0m";
    constexpr const char* BOLD    = "\033[1m";
    constexpr const char* RED     = "\033[91m";
    constexpr const char* GREEN   = "\033[92m";
    constexpr const char* YELLOW  = "\033[93m";
    constexpr const char* BLUE    = "\033[94m";
    constexpr const char* MAGENTA = "\033[95m";
    constexpr const char* CYAN    = "\033[96m";
#endif
    };

    class Application
    {
    private:
        std::optional<std::string> ReadFile(const char* filepath);
    public:
        static void PrintUsage();
        static void PrintNewHelp();
        static void PrintBuildHelp();
        static void PrintRunHelp();
        static void PrintCompileHelp();
        static void PrintGenBindHelp();

        static void CreateProject(const std::string& name, const std::string& type);
    };
};