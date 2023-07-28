#include "Application.h"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <stack>
#include <map>

#include <cstring>
#include <cstdlib>

namespace Iridis
{
    static constexpr const char* exeSourceCode = "io :: import!(\"std.io\")\n\nmain :: proc()\n{\n    io.println(\"Hello, World!\")\n}\n";
    static constexpr const char* libSourceCode = "Add :: proc(num1: i32, num2: i32) -> i32\n{\n    return num1 + num2\n}\n";

    std::optional<std::string> Application::ReadFile(const char* filepath)
    {
        std::ifstream file(filepath);

        if (!file)
            return std::nullopt;
        
        file.seekg(0, std::ios::end);
        const std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::string buffer;
        buffer.reserve(size);
        buffer.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(0));
        
        return buffer;
    }

    void Application::CreateProject(const std::string& name, const std::string& type)
    {
        namespace fs = std::filesystem;
        
        fs::create_directories(name + "/src");
        fs::permissions(name, fs::perms::others_all, fs::perm_options::remove);
        fs::permissions(name + "/src", fs::perms::others_all, fs::perm_options::remove);
        fs::current_path(name);

        std::ofstream projectConfig("iridis.toml");
        if (!projectConfig)
        {
            PrintError << "Could not write config file.\n";
            return;
        }

        projectConfig << "[project]\n"
                         "name = \"" << name << "\"\n"
                         "type = \"" << type << "\"\n"
                         "version = \"0.0.1\"\n"
                         "\n"
                         "[build_options]\n"
                         "source_directory = \"src\"\n"
                         "output_directory = \"bin\"\n";
        projectConfig.close();
        
        std::ofstream sourceFile(type == "exe" ? "src/main.iridis" : "src/" + name + ".iridis");
        if (!sourceFile)
        {
            PrintError << "Could not open source file.\n";
            return;
        }
        
        const char* sourceCode = (type == "exe" ? exeSourceCode : libSourceCode);
        sourceFile.rdbuf()->pubsetbuf(const_cast<char*>(sourceCode), std::strlen(sourceCode));
        sourceFile << sourceCode;
        sourceFile.close();
   }

    int Application::CompileFile(const std::string& path, const CompileOptions& compileOptions)
    {
        // TODO(Hachem): Implement Compiler
        std::cout << "Compiling `" << path << "` as `" << compileOptions.outputName << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return 0;
    }

    CompilationResult Application::CompileProject(const std::string& path, CompileOptions& compileOptions)
    {
        std::ifstream projectConfig(path + "/iridis.toml");
        if (!projectConfig)
            return CompilationResult::NoProject;

        // TODO(Hachem) Read TOML data, and replace `outputLocation` and `outputName`

        namespace fs = std::filesystem;

        std::string outputLocation = path + "/bin/";
        std::string sourceLocation = path + "/src/";
        std::string profileLocation;

        switch (compileOptions.profile)
        {
        case CompileOptions::CompilationProfiles::Debug:        profileLocation = "debug/";        break;
        case CompileOptions::CompilationProfiles::Release:      profileLocation = "release/";      break;
        case CompileOptions::CompilationProfiles::Distribution: profileLocation = "distribution/"; break;
        }
       
        std::string targetPath = outputLocation+profileLocation+compileOptions.outputName;

        fs::create_directories(targetPath);
        fs::permissions(outputLocation,                 fs::perms::others_all, fs::perm_options::remove);
        fs::permissions(outputLocation+profileLocation, fs::perms::others_all, fs::perm_options::remove);
        fs::permissions(targetPath,                     fs::perms::others_all, fs::perm_options::remove);

        std::stack<fs::path> directories;
        directories.push(fs::path(sourceLocation));

        while (!directories.empty())
        {
            fs::path current = directories.top();
            directories.pop();

            if (fs::is_directory(current))
            {
                fs::path newDestination = fs::path(targetPath)/current.lexically_relative(fs::path(sourceLocation));
                fs::create_directories(newDestination);

                for (const auto& entry : fs::directory_iterator(current))
                    if (fs::is_directory(entry))
                        directories.push(entry);
            }
        }

        std::map<std::string, std::string> sourceMapping; 

        for (const auto& entry : fs::recursive_directory_iterator(sourceLocation))
            if (fs::is_regular_file(entry) && entry.path().extension() == ".iridis")
            {
                std::string sourceFilePath = entry.path().string();

                std::string relativePath = entry.path().lexically_relative(fs::path(sourceLocation)).string();
                std::string targetFilePath = fs::path(targetPath + "/" + relativePath).replace_extension(".o");
                sourceMapping[sourceFilePath] = targetFilePath;
            }

        for (const auto& filePair : sourceMapping)
        {
            compileOptions.outputName = filePair.second;
            CompileFile(filePair.first, compileOptions);
        }

        return CompilationResult::Success;
    }

    int Application::RunProject(const std::string& path, CompileOptions& compileOptions)
    {
        int compile = (int) CompileProject(path, compileOptions);
        if (compile != 0)
            return compile;
        return 0;
    }
};
