#pragma once

#include "Common.h"

typedef enum CompilationProfile_t
{
    CompilationProfile_Debug,
    CompilationProfile_Release,
    CompilationProfile_Distribution,
} CompilationProfile;

typedef struct BuildConfiguration_t
{
    char* targetName;

    char** includeDirectories;
    char** importDirectories;
    
    CompilationProfile compilationProfile;
} BuildConfiguration;

typedef struct ProjectConfiguration_t
{
    char* name;
    char* type;
    char* version;
    char** authors;

    char* projectPath;
    char* sourceDirectoryPath;
    char* outputDirectoryPath;
    BuildConfiguration buildConfiguration;
} ProjectConfiguration;

bool ReadProjectConfiguration(ProjectConfiguration* output, char* projectPath);
void DestroyProjectConfiguration(ProjectConfiguration* configuration);

void GenerateProject(ProjectConfiguration* configuration);
bool BuildProject(ProjectConfiguration* configuration);
void RunProject(ProjectConfiguration* configuration);
void TestProject(ProjectConfiguration* configuration);
void CleanProject(ProjectConfiguration* configuration);
