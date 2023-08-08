#include "Utils.h"
#include "Log.h"

#include <stdio.h>
#include <stdlib.h>

char* ReadFile(const char* path)
{
    FILE* file = fopen(path, "r");

    if (!file)
    {
        LOG_ERROR("Could not open file @ `%s`.", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size_t bufferSize = ftell(file);
    rewind(file);

    char* buffer = (char*) malloc(bufferSize);
    fread(buffer, bufferSize, 1, file);
    buffer[bufferSize] = 0;

    fclose(file);
    return buffer;
}
