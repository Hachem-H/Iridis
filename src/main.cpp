#include "CMDInterface.h"
#include "Application.h"
#include "Log.h"

#include <unordered_map>
#include <string>

int main(int argc, char** argv)
{
    Iridis::Logger::Init();
    Iridis::ConsoleColors::Enable();

    if (argc < 2)
    {
        Iridis::Usages::PrintUsage();
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
    
    IRIDIS_ERROR("Unknown command: `{}`\n", argv[1]);
    Iridis::Usages::PrintUsage();
}
