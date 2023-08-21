#include "Platform.h"
#include "Utils.h"
#include "Log.h"

#include <stdio.h>
#include <stdlib.h>

#include <stb_ds.h>

char* ReadFile(const char* path)
{
    FILE* file = fopen(path, "r");

    if (!file)
    {
        LOG_ERROR("Could not open file @ `%s`.", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    usize bufferSize = ftell(file);
    rewind(file);

    char* buffer = (char*) malloc(bufferSize);
    fread(buffer, bufferSize, 1, file);
    buffer[bufferSize] = 0;

    fclose(file);
    return buffer;
}

char* GetFileName(const char* path)
{
    const char* lastSlash = strrchr(path, '/');
    const char* fileName = (lastSlash != NULL) ? lastSlash + 1 : path;
    return (char*)fileName;
}

bool EndsWith(const char* string, const char* suffix)
{
    usize stringLength = strlen(string);
    usize suffixLength = strlen(suffix);
    if (stringLength < suffixLength)
        return false;
    return strcmp(string + (stringLength-suffixLength), suffix) == 0;
}

void RecursivelyGetDirectories(const char* path, char*** directories)
{
#if defined(IRIDIS_WINDOWS)
    WIN32_FIND_DATA findFileData;
    HANDLE fileHandle = FindFirstFile(strcat(strcat(strdup(path), "\\"), "*"), &findFileData);

    if (fileHandle == INVALID_HANDLE_VALUE)
        return;

    do
    {
        if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0)
            continue;

        char fullpath[MAX_PATH];
        snprintf(fullpath, sizeof(fullpath), "%s\\%s", path, findFileData.cFileName);

        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            stbds_arrput(*directories, strdup(fullpath));
            RecursivelyGetDirectories(fullpath, directories);
        }
    } while (FindNextFile(fileHandle, &findFileData) != 0);

    FindClose(fileHandle);
#elif defined(IRIDIS_UNIX)
    DIR* directory = opendir(path);
    if (!directory)
        return;

    struct dirent* entry;
    while ((entry = readdir(directory)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char fullpath[PATH_MAX];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        struct stat statbuf;
        if (stat(fullpath, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
        {
            stbds_arrput(*directories, strdup(fullpath));
            RecursivelyGetDirectories(fullpath, directories);
        }
    }

    closedir(directory);
#endif
}

void RecursivelyGetFiles(const char* path, const char* extention, char*** files)
{
#if defined(IRIDIS_WINDOWS)
    WIN32_FIND_DATA findFileData;
    HANDLE findHandle = FindFirstFile(strcat(strcat(path, "\\*"), extention), &findFileData);

    if (findHandle == INVALID_HANDLE_VALUE)
        return;

    do
    {
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            continue;

        char fullPath[MAX_PATH];
        snprintf(fullPath, sizeof(fullPath), "%s\\%s", path, findFileData.cFileName);
        arrput(*files, strdup(fullPath));
    } while (FindNextFile(findHandle, &findFileData) != 0);

    FindClose(findHandle);
#elif defined(IRIDIS_UNIX)
    DIR* directory = opendir(path);
    if (directory == NULL)
        return;

    struct dirent* entry;
    while ((entry = readdir(directory)) != NULL)
    {
        if (entry->d_type == DT_REG && EndsWith(entry->d_name, extention))
        {
            char fullPath[PATH_MAX];
            snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);
            arrput(*files, strdup(fullPath));
        }
    }
#endif
}

