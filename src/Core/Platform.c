#include "Platform.h"

#if defined(IRIDIS_WINDOWS)
    #include <windows.h>
#elif defined(IRIDIS_LINUX) || defined(IRIDIS_DARWIN)
    #include <sys/stat.h>
    #include <unistd.h>
#endif

bool MakeDirectory(const char* path)
{
#if defined(IRIDIS_WINDOWS)
    if (CreateDirectory(path, NULL))
        return true;
#elif defined(IRIDIS_LINUX) || defined(IRIDIS_DARWIN)
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
#elif defined(IRIDIS_LINUX) || defined(IRIDIS_DARWIN)
    if (rmdir(path) == 0)
        return true;
#endif

    return false;
}

void EnableConsoleColors()
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
