#pragma once

#include <vector>
#include <string>
#include <optional>

#define PrintError std::cout << Iridis::TerminalColors::BOLD << Iridis::TerminalColors::RED << "ERROR: " << Iridis::TerminalColors::RESET

namespace Iridis
{
    namespace TerminalColors
    {
        // TODO(Hachem): For windows, use the Win32 API and have a better Terminal Abstraction.
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
    
    struct CompileOptions
    {
        enum class CompilationProfiles
        {
            Debug, Release, Distribution,
        };

        std::vector<std::string> importDirectories;
        std::vector<std::string> includeDirectories;
        
        CompilationProfiles profile = CompilationProfiles::Debug;
        std::string outputName = "output";
    };

    enum class CompilationResult
        : int
    {
        CompileError = -1,
        LinkerError = -2,
        NoProject = -3,
        Success = 0,
    };

    struct ProjectConfiguration
    {
        std::string projectName;
        std::string projectType;
        std::string version;

        std::string sourceDirectory;
        std::string outputDirectory;
    };

    class Application
    {
    private:
        static std::optional<std::string> ReadFile(const char* filepath);
        static ProjectConfiguration ReadProjectConfiguration(const std::string& path);
    public:
        static int CompileFile(const std::string& path, const CompileOptions& compileOptions);
        static CompilationResult CompileProject(const std::string& path, CompileOptions& compileOptions);
        static int RunProject(const std::string& path, CompileOptions& compileOptions);

        static void CreateProject(const std::string& name, const std::string& type);
    };
};
