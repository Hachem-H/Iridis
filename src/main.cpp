#include "Application.h"
#include "CMDInterface.h"

#include <unordered_map>
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        Iridis::Application::PrintUsage();
        return -1;
    }

    std::unordered_map<std::string, int (*)(int, char*[])> commands =
    {
        { "help",    Iridis::CommandLine::Help,    },
        { "new",     Iridis::CommandLine::New,     },
        { "build",   Iridis::CommandLine::Build,   },
        { "run",     Iridis::CommandLine::Run,     },
        { "compile", Iridis::CommandLine::Compile, },
        { "genbind", Iridis::CommandLine::GenBind, },
    };

    auto command = commands.find(argv[1]);
    if (command != commands.end())
        return command->second(argc, argv);
    
    PrintError << "Unknown command: `" << argv[1] << "`\n\n";
    Iridis::Application::PrintUsage();
}
