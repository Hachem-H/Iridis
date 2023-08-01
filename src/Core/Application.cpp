#include "Application.h"
#include "Log.h"

#include "Backend/Parser.h"
#include "Backend/Lexer.h"
#include "Backend/Token.h"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <future>
#include <vector>
#include <stack>
#include <map>

#include <cstring>
#include <cstdlib>

#include <toml.hpp>

namespace Iridis
{
    static constexpr const char* exeSourceCode = "io :: import!(\"std.io\")\n\nmain :: proc()\n{\n    io.println(\"Hello, World!\")\n}\n";
    static constexpr const char* libSourceCode = "Add :: proc(num1: i32, num2: i32) -> i32\n{\n    return num1 + num2\n}\n";

    std::optional<std::wstring> Application::ReadFile(const char* filepath)
    {
        std::wifstream file(filepath);
        
        if (!file)
            return std::nullopt;
        
        file.seekg(0, std::ios::end);
        const std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::wstring buffer;
        buffer.reserve(size);
        buffer.assign(std::istreambuf_iterator<wchar_t>(file), std::istreambuf_iterator<wchar_t>());
        
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
            IRIDIS_ERROR("Could not write config file.");
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
            IRIDIS_ERROR("Could not open source file.");
            return;
        }
        
        const char* sourceCode = (type == "exe" ? exeSourceCode : libSourceCode);
        sourceFile.rdbuf()->pubsetbuf(const_cast<char*>(sourceCode), std::strlen(sourceCode));
        sourceFile << sourceCode;
        sourceFile.close();
    }

    ProjectConfiguration Application::ReadProjectConfiguration(const std::string& path)
    {
        ProjectConfiguration result;
        std::string configPath = path + "/iridis.toml";

        const auto data = toml::parse(configPath);
        const auto projectTable = toml::find(data, "project");
        const auto buildOptionsTable = toml::find(data, "build_options");

        result.projectName = toml::find<std::string>(projectTable, "name");
        result.projectType = toml::find<std::string>(projectTable, "type");
        result.version = toml::find<std::string>(projectTable, "version");

        result.sourceDirectory = toml::find<std::string>(buildOptionsTable, "source_directory");
        result.outputDirectory = toml::find<std::string>(buildOptionsTable, "output_directory");

        return result;
    }

    int Application::CompileFile(const std::string& path, const CompileOptions& compileOptions)
    {
        std::optional<std::wstring> file = ReadFile(&path[0]);

        if (!file)
        {
            IRIDIS_CORE_ERROR("Could not load file");
            return -1;
        }

        std::vector<Token> tokens =  Lexer::Tokenize(*file);
        Parser parser = Parser(tokens);
        parser.Parse();
        
        return 0;
    }

    CompilationResult Application::CompileProject(const std::string& path, CompileOptions& compileOptions)
    {
        using namespace ConsoleColors;
        namespace fs = std::filesystem;

        if (!fs::exists(path + "/iridis.toml"))
            return CompilationResult::NoProject;

        ProjectConfiguration projectConfig = ReadProjectConfiguration(path);

        std::string outputLocation = path + "/" + projectConfig.outputDirectory + "/";
        std::string sourceLocation = path + "/" + projectConfig.sourceDirectory + "/";
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

        std::vector<std::future<int>> compilationFutures;
        compilationFutures.reserve(sourceMapping.size());

        for (const auto& filePair : sourceMapping)
        {  
            auto compileOptionsCopy = compileOptions;

            compilationFutures.emplace_back(std::async(std::launch::async, 
                [filePair, compileOptions = std::move(compileOptionsCopy)]() mutable 
                {
                    compileOptions.outputName = filePair.second;
                    return CompileFile(filePair.first, compileOptions);
                }));
        }

        int totalFiles = compilationFutures.size();
        for (auto it = sourceMapping.begin(); it != sourceMapping.end(); ++it)
        {
            const std::string& filename = it->first;

            int currentFile = std::distance(sourceMapping.begin(), it);
            float progress = static_cast<float>(currentFile + 1) / totalFiles;
            int barWidth = 50;
            int progressWidth = static_cast<int>(progress * barWidth);

            std::cout << BOLD << "\r[" << RESET << GREEN << std::string(progressWidth, '=')
                      << std::string(barWidth - progressWidth, ' ')  << RESET << BOLD
                      << "] " << RESET << std::setw(3) << static_cast<int>(progress * 100) 
                      << "% " << ITALIC << filename << RESET << " ";
            std::cout.flush();

            int compilationResult = compilationFutures[currentFile].get();
            if (compilationResult != 0)
            {
                std::cout << std::endl;
                IRIDIS_ERROR("`{}` was not compiled successfully!", projectConfig.projectName);
                return CompilationResult::CompileError;
            }
        }

        std::cout << std::endl;
        IRIDIS_INFO("`{}` compiled successfully!", projectConfig.projectName);
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
