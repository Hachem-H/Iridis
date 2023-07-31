#pragma once

#include <vector>
#include <string>
#include <optional>

namespace Iridis
{
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
        static std::optional<std::wstring> ReadFile(const char* filepath);
        static ProjectConfiguration ReadProjectConfiguration(const std::string& path);
    public:
        static int CompileFile(const std::string& path, const CompileOptions& compileOptions);
        static CompilationResult CompileProject(const std::string& path, CompileOptions& compileOptions);
        static int RunProject(const std::string& path, CompileOptions& compileOptions);

        static void CreateProject(const std::string& name, const std::string& type);
    };
};
