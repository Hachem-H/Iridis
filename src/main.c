#include "Common.h"

#include "Core/ProjectManager.h"
#include "Core/CMDInterface.h"
#include "Core/Platform.h"
#include "Core/Log.h"

#include "Backend/Token.h"
#include "Backend/Lexer.h"

#include <stb_ds.h>

i32 main(i32 argc, char** argv)
{
    EnableConsoleColors();

    if (argc < 2)
    {
        PrintUsage();
        return -1;
    }

    CommandEntry commandEntries[] =
    {
        { "help",    CMDLineHelp    },
        { "new",     CMDLineNew     },
        { "build",   CMDLineBuild   },
        { "run",     CMDLineRun     },
        { "compile", CMDLineCompile },
        { "genbind", CMDLineGenBind },
    };

    for (usize i = 0; i < sizeof(commandEntries)/sizeof(CommandEntry); i++)
    {
        if (streql(argv[1], commandEntries[i].command))
            return commandEntries[i].callback(argc, argv);
    }

    LOG_ERROR("Unknown comand: `%s`\n", argv[1]);
    PrintUsage();
    return -1;
}

