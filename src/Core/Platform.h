#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #define IRIDIS_WINDOWS
#elif defined(__APPLE__)
    #define IRIDIS_DARWIN
#elif defined(__linux__)
    #define IRIDIS_LINUX
#else
    #error "Unsupported platform"
#endif

#include <stdbool.h>

bool MakeDirectory(const char* path);
bool RemoveDirectory(const char* path);
void EnableConsoleColors();
