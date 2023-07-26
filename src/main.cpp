#include "Application.h"

#include <iostream>

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

    if (std::strcmp(argv[1], "help") == 0) 
    {
        Iridis::Application::PrintUsage();
        return 0;
    } 
    else if (std::strcmp(argv[1], "new") == 0)
    {
        if (argc == 4)
        {
            using namespace Iridis::TerminalColors;

            std::string type = argv[2];
            std::string name = argv[3];

            if (type != "exe" && type != "lib")
            {
                PrintError << "Unknown project type: `" << type << "`\n";
                std::cout << "       You can create an executable (exe) or a library (lib).\n\n";
                return -1;
            }

            if (!IsValidProjectName(name))
            {
                PrintError << "`" << name << "` is not a valid project name.\n\n"; 
                return -1;
            }

            Iridis::Application::CreateProject(name, type);
        } else
            Iridis::Application::PrintNewHelp();
        return 0;
    }
    else if (std::strcmp(argv[1], "build") == 0)
    {
        Iridis::Application::PrintBuildHelp();
        return 0;
    }
    else if (std::strcmp(argv[1], "run") == 0)
    {
        Iridis::Application::PrintRunHelp();
        return 0;
    }
    else if (std::strcmp(argv[1], "compile") == 0)
    {
        Iridis::Application::PrintCompileHelp();
        return 0;
    }
    else if (std::strcmp(argv[1], "genbind") == 0)
    {
        Iridis::Application::PrintGenBindHelp();
        return 0;
    }
    else
    {
        using namespace Iridis::TerminalColors;
        PrintError << "Unknown command: `" << argv[1] << "`\n\n";
        Iridis::Application::PrintUsage();
    }
}
