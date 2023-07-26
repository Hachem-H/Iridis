#include "Application.h"
#include "CMDInterface.h"

#include <iostream>

namespace Iridis
{
    static inline bool IsValidProjectName(const std::string& name)
    {
        for (char character : name)
            if (!(std::isalpha(character) || 
                  std::isdigit(character) || 
                  character == '_'))
            return false;
        return true;
    }


    namespace CommandLine
    {
        int Help(int argc, char* argv[])
        {
            Application::PrintUsage();
            return 0;
        }

        
        int New(int argc, char* argv[])
        {
            if (argc != 4)
            {
                Application::PrintRunHelp();
                return -1;
            }

            std::string type = argv[2];
            std::string name = argv[3];

            if (type != "exe" && type != "lib")
            {
                PrintError << "Unknown project type: `" << type << "`\n" << "       " <<
                              "You can create an executable (exe) or a library (lib).\n\n";
                return -1;
            }

            if (!IsValidProjectName(name))
            {
                PrintError << "`" << name << "` is not a valid project name.\n\n"; 
                return -1;
            }

            Application::CreateProject(name, type);
            std::cout << "Succefully created the " << (type == "exe" ? "executable" : "library")
                      << " `" << name << "`\n\n";
            return 0;
        }

        int Build(int argc, char* argv[])   { return 0 ; }
        int Run(int argc, char* argv[])     { return 0 ; }
        int Compile(int argc, char* argv[]) { return 0 ; }
        int GenBind(int argc, char* argv[]) { return 0 ; }
    };
};