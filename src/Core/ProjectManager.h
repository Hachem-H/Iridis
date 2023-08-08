#pragma once

#include <stdbool.h>

typedef enum CompilationProfile_t
{
    CompilationProfile_Debug,
    CompilationProfile_Release,
    CompilationProfile_Distribution,
} CompilationProfile;

typedef struct BuildConfiguration_t
{
    char* targetName;
    char** outputDirectory;

    char** includeDirectories;
    char** importDirectories;
    
    CompilationProfile compilationProfile;
} BuildConfiguration;

typedef struct ProjectConfiguration_t
{
    char* name;
    char* version;
    char** authors;

    char* projectPath;
    BuildConfiguration buildConfiguration;
} ProjectConfiguration;

bool ReadProjectConfiguration(ProjectConfiguration* output, char* projectPath);
void DestroyProjectConfiguration(ProjectConfiguration* configuration);

void GenerateProject(ProjectConfiguration configuration);
void BuildProject(ProjectConfiguration configuration);
void RunProject(ProjectConfiguration configuration);
void TestProject(ProjectConfiguration configuration);
void CleanProject(ProjectConfiguration configuration);
