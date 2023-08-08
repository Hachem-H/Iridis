#pragma once

typedef int (*CommandFunction)(int, char*[]);

typedef struct CommandEntry_t
{
    char* command;
    CommandFunction callback;
} CommandEntry;

int CMDLineHelp(int argc, char* argv[]);
int CMDLineNew(int argc, char* argv[]);
int CMDLineBuild(int argc, char* argv[]);
int CMDLineRun(int argc, char* argv[]);
int CMDLineCompile(int argc, char* argv[]);
int CMDLineGenBind(int argc, char* argv[]);

void PrintUsage();
void PrintNewHelp();
void PrintBuildHelp();
void PrintRunHelp();
void PrintCompileHelp();
void PrintGenBindHelp();
