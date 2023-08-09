#pragma once

#include <stdio.h>

static const char* const CONSOLE_MODE_RESET     = "\033[0m";
static const char* const CONSOLE_MODE_BOLD      = "\033[1m";
static const char* const CONSOLE_MODE_UNDERLINE = "\033[4m";
static const char* const CONSOLE_MODE_ITALIC    = "\033[3m";

static const char* const CONSOLE_COLORS_RED       = "\033[91m";
static const char* const CONSOLE_COLORS_GREEN     = "\033[92m";
static const char* const CONSOLE_COLORS_YELLOW    = "\033[93m";
static const char* const CONSOLE_COLORS_BLUE      = "\033[94m";
static const char* const CONSOLE_COLORS_MAGENTA   = "\033[95m";
static const char* const CONSOLE_COLORS_CYAN      = "\033[96m";

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
