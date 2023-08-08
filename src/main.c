#include "Core/ProjectManager.h"
#include "Core/Platform.h"
#include "Core/Log.h"

#include "Backend/Token.h"
#include "Backend/Lexer.h"

int main(int argc, char** argv)
{
    EnableConsoleColors();

    ProjectConfiguration configuration;

    configuration.name    = argv[1];
    configuration.type    = "exe";
    configuration.version = "0.0.1";

    configuration.sourceDirectoryPath = "src";
    configuration.outputDirectoryPath = "bin";

    GenerateProject(&configuration);
}
