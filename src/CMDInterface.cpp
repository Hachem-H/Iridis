#include "CMDInterface.h"
#include "Application.h"
#include "Log.h"

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
            Usages::PrintUsage();
            return 0;
        }
       
        int New(int argc, char* argv[])
        {
            // TODO(Hachem): Check if project already exists
            if (argc != 4)
            {
                Usages::PrintNewHelp();
                return -1;
            }

            std::string type = argv[2];
            std::string name = argv[3];

            if (type != "exe" && type != "lib")
            {
                IRIDIS_ERROR("Unknown project type: `{}`\n         "
                             "You can create an executable (exe) or a library (lib).\n");
                return -1;
            }

            if (!IsValidProjectName(name))
            {
                IRIDIS_ERROR("`{}` is not a valid project name.\n", name); 
                return -1;
            }

            Application::CreateProject(name, type);
            IRIDIS_INFO("Succesffuly create the {} `{}`\n", (type == "exe" ? "executable" : "library"), name);
            return 0;
        }

        int Build(int argc, char* argv[])
        {
            std::string path = ".";
            if (argc == 3)
                path = argv[2];
            
            CompileOptions options = {};

            return (int)Application::CompileProject(path, options);
        }

        int Run(int argc, char* argv[])     { return 0 ; }
        int Compile(int argc, char* argv[]) { return 0 ; }
        int GenBind(int argc, char* argv[]) { return 0 ; }
    };

    namespace Usages
    {
        using namespace TerminalColors;

        void PrintUsage()
        {
            std::cout << GREEN << "Usage: iridis [command] [options]\n\n" << RESET;

            std::cout << BLUE << "Commands:" << RESET << "\n";
            std::cout << BOLD << "  new" << RESET << " [exe|lib] [name] : Creates a new project of the specified type.\n";
            std::cout << BOLD << "  build" << RESET << " [project]      : Build the specified project (the argument is optional).\n";
            std::cout << BOLD << "  run" << RESET << " [project]        : Runs the specified project (the argument is optional).\n";
            std::cout << BOLD << "  compile" << RESET << " [file]       : Compiles the specified file.\n";
            std::cout << BOLD << "  genbind" << RESET << " [file]       : Generates C header files from the iridis file.\n\n";

            std::cout << CYAN << "Options:\n";
            std::cout << BOLD << "  -h, --help" << RESET << "           : Display this help message and quits.\n";
            std::cout << BOLD << "  -v, --version" << RESET << "        : Display the version of the Iridis Compiler.\n\n";

            std::cout << YELLOW << "Note:\n";
            std::cout << "  run " << BOLD << "`iridis [command] help`" << RESET << " to get additional help information for each command.\n";
        }

        void PrintNewHelp()
        {
            std::cout << GREEN << "Usage: iridis new [type] [name]\n\n" << RESET;

            std::cout << "Create a new project of the specified type in a directory with the given name,\n";
            std::cout << "which includes an iridis.tml and a src/main.iridis file.\n\n";

            std::cout << CYAN << "Options:\n" << RESET;
            std::cout << BOLD << "  [type]" << RESET << " : Use 'lib' for a library and 'exe' for an executable.\n";
            std::cout << BOLD << "  [name]" << RESET << " : Specify the name for your project.\n\n";

            std::cout << BLUE << "Commands:" << RESET << "\n";
            std::cout << BOLD << "  help" << RESET << "   : Display this help information.\n\n";
        }
            

        void PrintBuildHelp()
        {
            std::cout << GREEN << "Usage: iridis build [optional: project location]\n\n" << RESET;

            std::cout << "Builds a project. If the current directory is a project, it will build the current project.\n";
            std::cout << "If not, it will compile the project provided in the specified project location.\n\n";

            std::cout << CYAN << "Options:\n" << RESET;
            std::cout << BOLD << "  --release" << RESET << "                      : Build with the release profile (includes optimizations and symbols).\n";
            std::cout << BOLD << "  --distribution" << RESET << "                 : Build with the distribution profile (includes optimizations and no symbols).\n\n";

            std::cout << BOLD << "  --includes:[dir1],[dir2],[...]" << RESET << " : Specify directories for C header include files.\n";
            std::cout << BOLD << "  --imports:[dir1],[dir2],[...]" << RESET << "  : Specify directories for external modules.\n";

            std::cout << BLUE << "Commands:\n" << RESET;
            std::cout << BOLD << "  help" << RESET << "                           : Display this help information.\n\n";
        }

        void PrintRunHelp()
        {
            std::cout << GREEN << "Usage: iridis run [optional: project location]\n\n" << RESET;

            std::cout << "Builds the project and runs the executable. If the current directory is a project,\n";
            std::cout << "it will build and run the current project. If not, it will compile the project provided\n";
            std::cout << "in the specified project location and then run the executable.\n\n";

            std::cout << CYAN << "Options:\n" << RESET;
            std::cout << BOLD << "  --release" << RESET << "                      : Build with the release profile (includes optimizations and symbols).\n";
            std::cout << BOLD << "  --distribution" << RESET << "                 : Build with the distribution profile (includes optimizations and no symbols).\n\n";

            std::cout << BOLD << "  --includes:[dir1],[dir2],[...]" << RESET << " : Specify directories for C header include files.\n";
            std::cout << BOLD << "  --imports:[dir1],[dir2],[...]" << RESET << "  : Specify directories for external modules.\n";

            std::cout << BLUE << "Commands:\n" << RESET;
            std::cout << BOLD << "  help" << RESET << "                           : Display this help information.\n\n";
        }

        void PrintCompileHelp()
        {
            std::cout << GREEN << "Usage: iridis compile [file]\n\n" << RESET;

            std::cout << "Compiles the specified individual file into an executable.\n";
            std::cout << "This command is used for compiling a single source file without creating a project.\n\n";

            std::cout << CYAN << "Options:\n" << RESET;
            std::cout << BOLD << "  --output:[name]" << RESET << "                : Specify the output name for the compiled executable.\n";
            std::cout << BOLD << "  --optimize" << RESET << "                     : Enable compiler optimizations.\n";
            std::cout << BOLD << "  --debug" << RESET << "                        : Include debugging symbols in the executable.\n\n";

            std::cout << BOLD << "  --includes:[dir1],[dir2],[...]" << RESET << " : Specify directories for C header include files.\n";
            std::cout << BOLD << "  --imports:[dir1],[dir2],[...]" << RESET << "  : Specify directories for external modules.\n";

            std::cout << BLUE << "Commands:\n" << RESET;
            std::cout << BOLD << "  help" << RESET << "                           : Display this help information.\n\n";
        }

        void PrintGenBindHelp()
        {
            std::cout << GREEN << "Usage: iridis genbind [file] --output [header file]\n\n" << RESET;

            std::cout << "Generates header bindings from the specified Iridis file to C in a header file.\n\n";

            std::cout << CYAN << "Options:\n" << RESET;
            std::cout << BOLD << "  --output:[header file]" << RESET << " : Specify the output header file where bindings will be generated.\n\n";

            std::cout << BLUE << "Commands:\n" << RESET;
            std::cout << BOLD << "  help" << RESET << "                   : Display this help information.\n\n";
        }
    };
};
