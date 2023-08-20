#pragma once

#include <stdbool.h>

char* ReadFile(const char* path);
char* GetFileName(const char* path);
bool EndsWith(const char* string, const char* suffix);
void RecursivelyGetDirectories(const char* path, char*** directories);
void RecursivelyGetFiles(const char* path, const char* extension, char*** files);
