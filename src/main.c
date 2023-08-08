#include "Core/ProjectManager.h"
#include "Core/CMDInterface.h"
#include "Core/Platform.h"
#include "Core/Log.h"

#include "Backend/Token.h"
#include "Backend/Lexer.h"

#include <stb_ds.h>

int main(int argc, char** argv)
{
    EnableConsoleColors();

    if (argc < 2)
    {
        PrintUsage();
        return -1;
    }

    CommandEntry commandEntries[] =
    {
        (CommandEntry) { "help",    CMDLineHelp    },
        (CommandEntry) { "new",     CMDLineNew     },
        (CommandEntry) { "build",   CMDLineBuild   },
        (CommandEntry) { "run",     CMDLineRun     },
        (CommandEntry) { "compile", CMDLineCompile },
        (CommandEntry) { "genbind", CMDLineGenBind },
    };

    for (size_t i = 0; i < sizeof(commandEntries)/sizeof(CommandEntry); i++)
    {
        if (strcmp(argv[1], commandEntries[i].command) == 0)
            return commandEntries[i].callback(argc, argv);
    }

    LOG_ERROR("Unknown comand: `%s`\n", argv[1]);
    PrintUsage();
    return -1;
}

