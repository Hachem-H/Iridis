#pragma once

typedef i32 (*CommandFunction)(i32, char*[]);

typedef struct CommandEntry_t
{
    char* command;
    CommandFunction callback;
} CommandEntry;

i32 CMDLineHelp(i32 argc, char* argv[]);
i32 CMDLineNew(i32 argc, char* argv[]);
i32 CMDLineBuild(i32 argc, char* argv[]);
i32 CMDLineRun(i32 argc, char* argv[]);
i32 CMDLineCompile(i32 argc, char* argv[]);
i32 CMDLineGenBind(i32 argc, char* argv[]);

void PrintUsage(void);
void PrintNewHelp(void);
void PrintBuildHelp(void);
void PrintRunHelp(void);
void PrintCompileHelp(void);
void PrintGenBindHelp(void);
