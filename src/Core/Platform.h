#pragma once

#include "Common.h"

#if defined(_WIN32) || defined(_WIN64) || defined(__MINGW32__) || defined(__CYGWIN__)
    #define IRIDIS_WINDOWS
#elif defined(__APPLE__) || defined(__linux__)
    #define IRIDIS_UNIX

    #include <sys/types.h>
    #include <sys/stat.h>

    #include <pthread.h>
    #include <unistd.h>
    #include <dirent.h>
    #include <limits.h>
#else
    #error "Unsupported platform"
#endif

#include <stdio.h>
#include <string.h>

bool MakeDirectory(const char* path);
char* ChangeDirectory(const char* path);
bool DeleteDirectory(const char* path);

bool FileExists(const char* path);
void EnableConsoleColors(void);

typedef struct Thread_t
{
#if defined(IRIDIS_WINDOWS)
    void* handle;
#elif defined(IRIDIS_UNIX) 
    pthread_t thread;
#endif
} Thread;

bool InitializeThread(Thread* thread, void* callback(void*), void* argument);
bool JoinThread(Thread* thread);
