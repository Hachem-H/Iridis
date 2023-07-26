#include "Application.h"
#include "CMDInterface.h"

#include <iostream>
#include <memory>
#include <string>
#include <map>

#include <cstring>
#include <cctype>

static inline bool IsValidProjectName(const std::string& name)
{
    for (char character : name)
        if (!(std::isalpha(character) || std::isdigit(character) || character == '_'))
            return false;
    return !name.empty();
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        Iridis::Application::PrintUsage();
        return -1;
    }

    std::map<std::string, std::unique_ptr<Iridis::CommandLine::Command>> commands;
    commands["help"]    = std::make_unique<Iridis::CommandLine::HelpCommand>();
    commands["new"]     = std::make_unique<Iridis::CommandLine::NewCommand>();
    commands["build"]   = std::make_unique<Iridis::CommandLine::BuildCommand>();
    commands["run"]     = std::make_unique<Iridis::CommandLine::RunCommand>();
    commands["compile"] = std::make_unique<Iridis::CommandLine::CompileCommand>();
    commands["genbind"] = std::make_unique<Iridis::CommandLine::GenBindCommand>();

    auto command = commands.find(argv[1]);
    if (command != commands.end())
        return command->second->execute(argc, argv);
    
    PrintError << "Unknown command: `" << argv[1] << "`\n\n";
    Iridis::Application::PrintUsage();
}
