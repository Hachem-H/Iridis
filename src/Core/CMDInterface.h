#pragma once

typedef int (*CommandFunction)(int, char*[]);

typedef struct CommandEntry_t
{
    char* command;
    CommandFunction callback;
} CommandEntry;

int CMDLine_Help(int argc, char* argv[]);
int CMDLine_New(int argc, char* argv[]);
int CMDLine_Build(int argc, char* argv[]);
int CMDLine_Run(int argc, char* argv[]);
int CMDLine_Compile(int argc, char* argv[]);
int CMDLine_GenBind(int argc, char* argv[]);

void PrintUsage();
void PrintNewHelp();
void PrintBuildHelp();
void PrintRunHelp();
void PrintCompileHelp();
void PrintGenBindHelp();
