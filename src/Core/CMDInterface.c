#include "ProjectManager.h"
#include "CMDInterface.h"
#include "Utils.h"
#include "Log.h"

#include "Backend/Compiler.h"

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

static inline bool IsValidProjectName(const char* name)
{
    for (int i = 0; i < strlen(name); i++)
        if (!isalpha(name[i]) || !isdigit(name[i]) || name[i] != '_')
            return false;
    return true;
}

int CMDLineHelp(int argc, char* argv[])
{
    PrintUsage();
    return 0;
}

int CMDLineNew(int argc, char* argv[])
{
    if (argc != 4)
    {
        PrintNewHelp();
        return -1;
    }

    if (strcmp(argv[2], "help") == 0)
    {
        PrintNewHelp();
        return 0;
    }

    char* type = argv[2];
    char* name = argv[3];

    if (strcmp(type, "exe") != 0 && strcmp(type, "lib") != 0)
    {
        LOG_ERROR("Unknown project type: `%s`.\n         You can created an executable (exe) or a library (lib).", type);
        return -1;
    }

    if (!IsValidProjectName(name))
    {
        LOG_ERROR("`%s` is not a valid project name.", name);
        return -1;
    }

    ProjectConfiguration configuration;

    configuration.name = name;
    configuration.type = type;
    configuration.version = "0.0.1";

    configuration.sourceDirectoryPath = "src";
    configuration.outputDirectoryPath = "bin";

    GenerateProject(&configuration);
    LOG_INFO("Successfully create the %s `%s`\n", strcmp(type, "exe") == 0 ? "executable" : "library", name);
    return 0;
}

int CMDLineBuild(int argc, char* argv[])
{
    return 0;
}

int CMDLineRun(int argc, char* argv[])
{
    return 0;
}

int CMDLineCompile(int argc, char* argv[])
{
    if (argc < 3)
    {
        PrintCompileHelp();
        return -1;
    }
    
    if (strcmp(argv[2], "help") == 0)
    {
        PrintCompileHelp();
        return 0;
    }

    char* sourceFilePath = argv[2];
    char* sourceCode = ReadFile(sourceFilePath);

    if (!sourceCode)
        return -1;

    int compileStatus = CompileSourceCode(sourceCode);

    free(sourceCode);
    return compileStatus;
}

int CMDLineGenBind(int argc, char* argv[])
{
    return 0;
}

void PrintUsage()
{
    printf("%sUsage: iridis [command] [options]\n\n%s", CONSOLE_COLORS_GREEN, CONSOLE_MODE_RESET);

    printf("%sCommands:%s\n", CONSOLE_COLORS_BLUE, CONSOLE_MODE_RESET);
    printf("%s  new%s [exe|lib] [name] : Creates a new project of the specified type.\n", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);
    printf("%s  build%s [project]      : Build the specified project (the argument is optional).\n", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);
    printf("%s  run%s [project]        : Runs the specified project (the argument is optional).\n", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);
    printf("%s  compile%s [file]       : Compiles the specified file.\n", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);
    printf("%s  genbind%s [file]       : Generates C header files from the iridis file.\n\n", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);

    printf("%sOptions:\n", CONSOLE_COLORS_CYAN);
    printf("%s  -h, --help%s           : Display this help message and quits.\n", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);
    printf("%s  -v, --version%s        : Display the version of the Iridis Compiler.\n\n", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);

    printf("%sNote:\n", CONSOLE_COLORS_YELLOW);
    printf("  run %s`iridis [command] help`%s to get additional help information for each command.\n", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);
}

void PrintNewHelp()
{
    printf("%sUsage: iridis new [type] [name]\n\n%s", CONSOLE_COLORS_GREEN, CONSOLE_MODE_RESET);

    printf("Create a new project of the specified type in a directory with the given name,\n");
    printf("which includes an iridis.tml and a src/main.iridis file.\n\n");

    printf("%sOptions:\n%s", CONSOLE_COLORS_CYAN, CONSOLE_MODE_RESET);
    printf("%s  [type]%s : Use 'lib' for a library and 'exe' for an executable.\n", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);
    printf("%s  [name]%s : Specify the name for your project.\n\n", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);

    printf("%sCommands:%s\n", CONSOLE_COLORS_BLUE, CONSOLE_MODE_RESET);
    printf("%s  help%s   : Display this help information.\n\n", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);
}

void PrintBuildHelp()
{
    printf("%sUsage: iridis build [optional: project location]\n\n%s", CONSOLE_COLORS_GREEN, CONSOLE_MODE_RESET);

    printf("Builds a project. If the current directory is a project, it will build the current project.\n");
    printf("If not, it will compile the project provided in the specified project location.\n\n");

    printf("%sOptions:\n%s", CONSOLE_COLORS_CYAN, CONSOLE_MODE_RESET);
    printf("%s  --release%s                      : Build with the release profile (includes optimizations and symbols).\n", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);
    printf("%s  --distribution%s                 : Build with the distribution profile (includes optimizations and no symbols).\n\n", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);

    printf("%s  --includes:[dir1],[dir2],[...]%s : Specify directories for C header include files.\n", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);
    printf("%s  --imports:[dir1],[dir2],[...]%s  : Specify directories for external modules.\n\n", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);

    printf("%sCommands:\n%s", CONSOLE_COLORS_BLUE, CONSOLE_MODE_RESET);
    printf("%s  help%s                           : Display this help information.\n\n", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);
}

void PrintRunHelp()
{
    printf("%sUsage: iridis run [optional: project location]\n\n%s", CONSOLE_COLORS_GREEN, CONSOLE_MODE_RESET);

    printf("Builds the project and runs the executable. If the current directory is a project,\n");
    printf("it will build and run the current project. If not, it will compile the project provided\n");
    printf("in the specified project location and then run the executable.\n\n");

    printf("%sOptions:\n%s", CONSOLE_COLORS_CYAN, CONSOLE_MODE_RESET);
    printf("%s  --release%s                      : Build with the release profile (includes optimizations and symbols).\n", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);
    printf("%s  --distribution%s                 : Build with the distribution profile (includes optimizations and no symbols).\n\n", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);

    printf("%s  --includes:[dir1],[dir2],[...]%s : Specify directories for C header include files.\n", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);
    printf("%s  --imports:[dir1],[dir2],[...]%s  : Specify directories for external modules.\n\n", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);

    printf("%sCommands:\n%s", CONSOLE_COLORS_BLUE, CONSOLE_MODE_RESET);
    printf("%s  help%s                           : Display this help information.\n\n", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);
}

void PrintCompileHelp()
{
    printf("%sUsage: iridis compile [file]\n\n%s", CONSOLE_COLORS_GREEN, CONSOLE_MODE_RESET);

    printf("Compiles the specified individual file into an executable.\n");
    printf("This command is used for compiling a single source file without creating a project.\n\n");

    printf("%sOptions:\n%s", CONSOLE_COLORS_CYAN, CONSOLE_MODE_RESET);
    printf("%s  --output:[name]%s                : Specify the output name for the compiled executable.\n", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);
    printf("%s  --release%s                      : Compile with the release profile (includes optimizations and symbols).\n", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);
    printf("%s  --distribution%s                 : Compile with the distribution profile (includes optimizations and no symbols).\n\n", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);

    printf("%s  --includes:[dir1],[dir2],[...]%s : Specify directories for C header include files.\n", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);
    printf("%s  --imports:[dir1],[dir2],[...]%s  : Specify directories for external modules.\n\n", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);

    printf("%sCommands:\n%s", CONSOLE_COLORS_BLUE, CONSOLE_MODE_RESET);
    printf("%s  help%s                           : Display this help information.\n\n", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);
}

void PrintGenBindHelp()
{
    printf("%sUsage: iridis genbind [file] --output [header file]\n\n%s", CONSOLE_COLORS_GREEN, CONSOLE_MODE_RESET);

    printf("Generates header bindings from the specified Iridis file to C in a header file.\n\n");

    printf("%sOptions:\n%s", CONSOLE_COLORS_CYAN, CONSOLE_MODE_RESET);
    printf("%s  --output:[header file]%s : Specify the output header file where bindings will be generated.\n\n", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);

    printf("%sCommands:\n%s", CONSOLE_COLORS_BLUE, CONSOLE_MODE_RESET);
    printf("%s  help%s                   : Display this help information.\n\n", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);
}
