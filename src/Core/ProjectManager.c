#include "ProjectManager.h"
#include "Platform.h"
#include "Log.h"

#include <string.h>
#include <stdlib.h>

bool ReadProjectConfiguration(ProjectConfiguration* output, char* projectPath)
{
    return false;
}

void DestroyProjectConfiguration(ProjectConfiguration* configuration)
{

}

void GenerateProject(ProjectConfiguration* configuration)
{
    size_t nameLength       = strlen(configuration->name);
    size_t sourcePathLength = nameLength + strlen(configuration->sourceDirectoryPath) + 2;
    size_t configPathLength = nameLength + strlen("/iridis.toml") + 1;
    size_t codePathLength   = sourcePathLength + strlen("/main.iridis") + 1;

    char* sourceDirectoryPath = (char*)malloc(sourcePathLength);
    char* configFilePath      = (char*)malloc(configPathLength);
    char* sourceCodePath      = (char*)malloc(codePathLength);

    snprintf(sourceDirectoryPath, sourcePathLength, "%s/%s", configuration->name, configuration->sourceDirectoryPath);
    snprintf(configFilePath, configPathLength, "%s/iridis.toml", configuration->name);
    snprintf(sourceCodePath, codePathLength, "%s/main.iridis", sourceDirectoryPath);

    MakeDirectory(configuration->name);
    MakeDirectory(sourceDirectoryPath);

    FILE* configFile = fopen(configFilePath, "w");
    fprintf(configFile, "[project]\n");
    fprintf(configFile, "name = \"%s\"\n",    configuration->name);
    fprintf(configFile, "type = \"%s\"\n",    configuration->type);
    fprintf(configFile, "version = \"%s\"\n\n", configuration->version);
    fprintf(configFile, "[build_configuration]\n");
    fprintf(configFile, "source_directory = \"%s\"\n", configuration->sourceDirectoryPath);
    fprintf(configFile, "output_directory = \"%s\"\n", configuration->outputDirectoryPath);
    fclose(configFile);

    FILE* sourceCodeFile = fopen(sourceCodePath, "w");
    fprintf(sourceCodeFile, "io :: import!(\"std.io\")\n\n");
    fprintf(sourceCodeFile, "main :: proc()\n");
    fprintf(sourceCodeFile, "{\n");
    fprintf(sourceCodeFile, "   io.println(\"Hello, World!\")\n");
    fprintf(sourceCodeFile, "}\n");
    fclose(sourceCodeFile);

    free(sourceDirectoryPath);
    free(configFilePath);
    free(sourceCodePath);
}

void BuildProject(ProjectConfiguration* configuration)
{

}

void RunProject(ProjectConfiguration* configuration)
{

}

void TestProject(ProjectConfiguration* configuration)
{

}

void CleanProject(ProjectConfiguration* configuration)
{

}
