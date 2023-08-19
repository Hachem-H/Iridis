#include "Platform.h"

bool MakeDirectory(const char* path)
{
#if defined(IRIDIS_WINDOWS)
    if (CreateDirectory(path, NULL))
        return true;
#elif defined(IRIDIS_UNIX)
    if (mkdir(path, 0777) == 0)
        return true;
#endif

    return false;
}
bool RemoveDirectory(const char* path)
{
#if defined(IRIDIS_WINDOWS)
    if (RemoveDirectory(path))
        return true;
#elif defined(IRIDIS_UNIX)
    if (rmdir(path) == 0)
        return true;
#endif

    return false;
}

char* ChangeDirectory(const char* path)
{
#if defined(IRIDIS_WINDOWS)
    char currentWorkingDirectory[MAX_PATH];
    DWORD currentWorkingDirectorySize = GetCurrentDirectory(MAX_PATH, buffer);
    if (SetCurrentDirectory(path) != 0)
        if (size != 0)
            return strdup(currentWorkingDirectory);
#elif defined(IRIDIS_UNIX)
    char currentWorkingDirectory[1024];
    if (getcwd(currentWorkingDirectory, sizeof(currentWorkingDirectory)) != NULL)
        if (chdir(path) == 0)
            return strdup(currentWorkingDirectory);
#endif

    return NULL;
}

void EnableConsoleColors(void)
{
#if defined(IRIDIS_WINDOWS)
    HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    if (outputHandle == INVALID_HANDLE_VALUE)
        return;

    DWORD consoleMode = 0;
    if (!GetConsoleMode(outputHandle, &consoleMode))
        return;

    consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(outputHandle, consoleMode))
        return;
#endif
}

bool FileExists(const char* path)
{
#if defined(IRIDIS_WINDOWS)
    DWORD attribute = GetFileAttribute(path);
    return (attribute != INVALID_FILE_ATTRIBUTES && !(attribute) & FILE_ATTRIBUTE_DIRECTORY));
#elif defined(IRIDIS_UNIX)
    return access(path, F_OK) == 0;
#endif
}

bool InitializeThread(Thread* thread, void* callback(void*), void* argument)
{
#if defined(IRIDIS_WINDOWS)
    thread->handle = CreateThread(NULL, 0, callback, argument, 0, NULL);
    if (thread->handle == NULL)
        return false;
#elif defined(IRIDIS_UNIX)
    if (pthread_create(&thread->thread, NULL, callback, argument) != 0)
        return false;
#endif
    return true;
}

bool JoinThread(Thread* thread)
{
#if defined(IRIDIS_WINDOWS)
    WaitForSingleObject(thread->handle, INFINITE);
    CloseHandle(thread->handle);
#elif defined(IRIDIS_UNIX)
    if (pthread_join(thread->thread, NULL) != 0)
        return false;
#endif
    return true;
}
