#include "Application.h"

#include <iostream>
#include <cstring>

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
        std::cout << BOLD << RED << "ERROR:" << RESET<<  " Unknown command: `" << argv[1] << "`\n\n";
        Iridis::Application::PrintUsage();
    }
}
