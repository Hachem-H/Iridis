#include "ProjectManager.h"
#include "Platform.h"
#include "Log.h"
#include "FS.h"


#include "Backend/Compiler.h"

#include <toml.h>
#include <stb_ds.h>

#include <string.h>
#include <stdlib.h>

bool ReadProjectConfiguration(ProjectConfiguration* output, char* projectPath)
{
    usize configFilepathLength = strlen(projectPath) + strlen("/iridis.toml")+2;
    char* configFilepath = alloc(char, configFilepathLength);
    snprintf(configFilepath, configFilepathLength, "%s/iridis.toml", projectPath);
    char* configFile = ReadFileContents(configFilepath);

    if (configFile == NULL)
    {
        LOG_ERROR("`%s` is not a valid Iridis project.", projectPath);
        return false;
    }

    free(configFilepath);

    char errorBuffer[500];
    toml_table_t* config = toml_parse(configFile, errorBuffer, sizeof(errorBuffer));
    free(configFile);
    
    if (!config)
    {
        LOG_ERROR("Could not parse `iridis.toml` : %s", errorBuffer);
        return false;
    }

    output->projectPath = strdup(projectPath);

    toml_table_t* projectConfig = toml_table_in(config, "project");
    toml_table_t* buildConfig   = toml_table_in(config, "build_configuration");
    
    if (!projectConfig)
    {
        LOG_ERROR("Missing [project] section in `iridis.toml`");
        return false;
    }

    if (!buildConfig)
    {
        LOG_ERROR("Missing [build_configuration] section in `iridis.toml`");
        return false;
    }

    {
        toml_datum_t  name    = toml_string_in(projectConfig, "name");
        toml_datum_t  type    = toml_string_in(projectConfig, "type");
        toml_datum_t  version = toml_string_in(projectConfig, "version");
        toml_array_t* authors = toml_array_in(projectConfig, "authors");

        if (!name.ok)
        {
            LOG_ERROR("Missing project name in `iridis.toml`");
            return false;
        }

        if (!type.ok)
        {
            LOG_ERROR("Missing project type in `iridis.toml`");
            return false;
        }

        output->name = strdup(name.u.s); free(name.u.s);
        output->type = strdup(type.u.s); free(type.u.s);
        output->buildConfiguration.targetName = strdup(output->name);

        if (!version.ok)
        {
            LOG_WARN("Missing project version in `iridis.toml` assuming version = 0.0.1");
            output->version = strdup("0.0.1");
        } 
        else
        {
            output->version = strdup(version.u.s);
            free(version.u.s);
        }

        output->authors = NULL;
        if (authors)
        {
            for (i32 i = 0; ;i++)
            {
                toml_datum_t author = toml_string_at(authors, i);
                if (!author.ok)
                    break;
                stbds_arrpush(output->authors, author.u.s);
                free(author.u.s);
            }

            free(authors);
        }
    } 

    {
        toml_datum_t sourceDirectoryPath = toml_string_in(buildConfig, "source_directory");
        toml_datum_t outputDirectoryPath = toml_string_in(buildConfig, "output_directory");

        if (!sourceDirectoryPath.ok)
        {
            LOG_ERROR("Missing source directory path in `iridis.toml`");
            return false;
        }

        if (!outputDirectoryPath.ok)
        {
            LOG_ERROR("Missing output directory path in `iridis.toml`");
            return false;
        }

        output->sourceDirectoryPath = strdup(sourceDirectoryPath.u.s); free(sourceDirectoryPath.u.s);
        output->outputDirectoryPath = strdup(outputDirectoryPath.u.s); free(outputDirectoryPath.u.s);
    
        toml_datum_t profile = toml_string_in(buildConfig, "profile");

        if (profile.ok)
        {
            char* profileString = profile.u.s;

            if (streql(profileString, "debug"))
                output->buildConfiguration.compilationProfile = CompilationProfile_Debug;
            else if (streql(profileString, "release"))
                output->buildConfiguration.compilationProfile = CompilationProfile_Release;
            else if (streql(profileString, "distribution"))
                output->buildConfiguration.compilationProfile = CompilationProfile_Distribution;
            else
            {
                LOG_ERROR("Unknown compilation profile: `%s`", profileString);
                return false;
            }

            free(profileString);
        } else
            output->buildConfiguration.compilationProfile = CompilationProfile_Debug;

        toml_array_t* includeDirectories = toml_array_in(buildConfig, "include_directories");
        toml_array_t* importDirectories  = toml_array_in(buildConfig, "import_directories");

        output->buildConfiguration.includeDirectories = NULL;
        output->buildConfiguration.importDirectories  = NULL;

        if (includeDirectories)
        {
            for (i32 i = 0; ;i++)
            {
                toml_datum_t directory = toml_string_at(includeDirectories, i);
                if (!directory.ok)
                    break;
                stbds_arrpush(output->buildConfiguration.includeDirectories, directory.u.s);
                free(directory.u.s);
            }

            free(includeDirectories);
        }

        if (importDirectories)
        {
            for (i32 i = 0; ;i++)
            {
                toml_datum_t directory = toml_string_at(importDirectories, i);
                if (!directory.ok)
                    break;
                stbds_arrpush(output->buildConfiguration.importDirectories, directory.u.s);
                free(directory.u.s);
            }

            free(importDirectories);
        }
    }

    toml_free(config);
    return true;
}

void DestroyProjectConfiguration(ProjectConfiguration* configuration)
{
    free(configuration->name);
    free(configuration->type);
    free(configuration->version);
    free(configuration->projectPath);
    free(configuration->sourceDirectoryPath);
    free(configuration->outputDirectoryPath);
    
    for (i32 i = 0; i < stbds_arrlen(configuration->authors); i++)
        free(configuration->authors[i]);
    stbds_arrfree(configuration->authors);

    free(configuration->buildConfiguration.targetName);

    for (i32 i = 0; i < stbds_arrlen(configuration->buildConfiguration.includeDirectories); i++)
        free(configuration->buildConfiguration.includeDirectories[i]);
    for (i32 i = 0; i < stbds_arrlen(configuration->buildConfiguration.importDirectories); i++)
        free(configuration->buildConfiguration.importDirectories[i]);
    
    stbds_arrfree(configuration->buildConfiguration.includeDirectories);
    stbds_arrfree(configuration->buildConfiguration.importDirectories);
}

void GenerateProject(ProjectConfiguration* configuration)
{
    usize nameLength       = strlen(configuration->name);
    usize sourcePathLength = nameLength + strlen(configuration->sourceDirectoryPath) + 2;
    usize configPathLength = nameLength + strlen("/iridis.toml") + 1;
    usize codePathLength   = sourcePathLength + strlen("/main.iridis") + 1;

    char* sourceDirectoryPath = alloc(char, sourcePathLength);
    char* configFilePath      = alloc(char, configPathLength);
    char* sourceCodePath      = alloc(char, codePathLength);

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

bool BuildProject(ProjectConfiguration* configuration)
{
    char* buildProfile;

    switch (configuration->buildConfiguration.compilationProfile)
    {
    case CompilationProfile_Debug:        buildProfile = "debug";        break;
    case CompilationProfile_Release:      buildProfile = "release";      break;
    case CompilationProfile_Distribution: buildProfile = "distribution"; break;
    default: break;
    }
    
    usize configPathLength = strlen(configuration->projectPath)+strlen("/iridis.toml")+1;
    usize outputPathLength = strlen(configuration->projectPath)+strlen(configuration->outputDirectoryPath)+2;
    usize sourcePathLength = strlen(configuration->projectPath)+strlen(configuration->sourceDirectoryPath)+2;
    usize targetPathLength = outputPathLength+strlen(buildProfile)+1;

    char* configPath = alloc(char, configPathLength);
    char* outputPath = alloc(char, outputPathLength);
    char* sourcePath = alloc(char, sourcePathLength);
    char* targetPath = alloc(char, targetPathLength);

    snprintf(configPath, configPathLength, "%s/iridis.toml", configuration->projectPath);
    snprintf(outputPath, outputPathLength, "%s/%s", configuration->projectPath, configuration->outputDirectoryPath);
    snprintf(sourcePath, sourcePathLength, "%s/%s", configuration->projectPath, configuration->sourceDirectoryPath);
    snprintf(targetPath, targetPathLength, "%s/%s", outputPath, buildProfile);

    MakeDirectory(outputPath);
    MakeDirectory(targetPath);

    char** sourceDirectories = NULL;
    char** sourceFiles       = NULL;

    char* currentWorkingDirectory = ChangeDirectory(sourcePath);
    RecursivelyGetDirectories(".", &sourceDirectories);
    RecursivelyGetFiles(".", ".iridis", &sourceFiles);
    ChangeDirectory(currentWorkingDirectory);
    free(currentWorkingDirectory);

    for (i32 i = 0; i < stbds_arrlen(sourceDirectories); i++)
    {
        usize pathLength = strlen(targetPath)+strlen(sourceDirectories[i])+2;
        char* pathBuffer = alloc(char, pathLength);
        snprintf(pathBuffer, pathLength, "%s/%s", targetPath, sourceDirectories[i]);
        MakeDirectory(pathBuffer);
        free(pathBuffer);
    }
    

    usize totalFiles = stbds_arrlen(sourceFiles);
    for (usize i = 0; i < totalFiles; i++)
    {
        usize outputFileLength = strlen(targetPath)+strlen(sourceFiles[i])+1;
        usize sourceFileLength = sourcePathLength+strlen(sourceFiles[i])+1;

        char* outputFile = alloc(char, outputFileLength);
        char* sourceFile = alloc(char, sourceFileLength);

        snprintf(outputFile, outputFileLength, "%s/%s", targetPath, sourceFiles[i]);
        snprintf(sourceFile, sourceFileLength, "%s/%s", sourcePath, sourceFiles[i]);

        outputFile[outputFileLength-strlen("iridis")+0] = 'o';
        outputFile[outputFileLength-strlen("iridis")+1] = 0;

        f32 progress = (f32)(i+1)/totalFiles;
        i32 barWidth = 50;
        i32 progressWidth = (progress*barWidth);

        printf("\r%s[%s%s", CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET, CONSOLE_COLORS_GREEN);
        for (i32 j = 0; j < progressWidth; j++)
            putchar('=');
        for (i32 j = 0; j < barWidth-progressWidth; j++)
            putchar(' ');
        printf("%s%s]%s", CONSOLE_MODE_RESET, CONSOLE_MODE_BOLD, CONSOLE_MODE_RESET);
        printf("  %d%% %s%s%s", (i32)(progress*100), CONSOLE_MODE_ITALIC, sourceFile, CONSOLE_MODE_RESET);

        if (i >= 1)
        {
            i32 size = (i32)strlen(sourceFiles[i-1])-(i32)strlen(sourceFiles[i]);
            if (size > 0)
            {
                for (i32 j = 0; j < size; j++)
                    putchar(' ');
                for (i32 j = 0; j < size; j++)
                    putchar('\b');
            }
        }

        fflush(stdout);

        CompileSourceCode(outputFile, sourceFile);

        free(outputFile);
        free(sourceFile);
    }

    putchar('\n');

    for (i32 i = 0; i < stbds_arrlen(sourceDirectories); i++)
        free(sourceDirectories[i]);
    for (i32 i = 0; i < stbds_arrlen(sourceFiles); i++)
        free(sourceFiles[i]);

    stbds_arrfree(sourceDirectories);
    stbds_arrfree(sourceFiles);

    free(configPath);
    free(outputPath);
    free(targetPath);
    free(sourcePath);

    return true;
}

void RunProject(ProjectConfiguration* configuration)
{
    BuildProject(configuration);
}

void TestProject(ProjectConfiguration* configuration)
{
    (void) configuration;
}

void CleanProject(ProjectConfiguration* configuration)
{
    DeleteDirectory(configuration->outputDirectoryPath);
}
