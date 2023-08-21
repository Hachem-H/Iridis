#pragma once

#include "Common.h"

#include <stdio.h>

global const char* const CONSOLE_MODE_RESET     = "\033[0m";
global const char* const CONSOLE_MODE_BOLD      = "\033[1m";
global const char* const CONSOLE_MODE_UNDERLINE = "\033[4m";
global const char* const CONSOLE_MODE_ITALIC    = "\033[3m";

global const char* const CONSOLE_COLORS_RED       = "\033[91m";
global const char* const CONSOLE_COLORS_GREEN     = "\033[92m";
global const char* const CONSOLE_COLORS_YELLOW    = "\033[93m";
global const char* const CONSOLE_COLORS_BLUE      = "\033[94m";
global const char* const CONSOLE_COLORS_MAGENTA   = "\033[95m";
global const char* const CONSOLE_COLORS_CYAN      = "\033[96m";

#define LOG_INFO(...) {                           \
    printf("[%s%sINFO%s]: ", CONSOLE_COLORS_CYAN, \
                             CONSOLE_MODE_BOLD,   \
                             CONSOLE_MODE_RESET); \
    printf(__VA_ARGS__); printf("\n"); }

#define LOG_DEBUG(...) {                           \
    printf("[%s%sDEBUG%s]: ", CONSOLE_COLORS_BLUE, \
                              CONSOLE_MODE_BOLD,   \
                              CONSOLE_MODE_RESET); \
    printf(__VA_ARGS__); printf("\n");}

#define LOG_WARN(...) {                             \
    printf("[%s%sWARN%s]: ", CONSOLE_COLORS_YELLOW, \
                             CONSOLE_MODE_BOLD,     \
                             CONSOLE_MODE_RESET);   \
    printf(__VA_ARGS__); printf("\n"); }

#define LOG_ERROR(...) {                           \
    printf("[%s%sERROR%s]: ", CONSOLE_COLORS_RED,  \
                              CONSOLE_MODE_BOLD,   \
                              CONSOLE_MODE_RESET); \
    printf(__VA_ARGS__); printf("\n"); }
