#pragma once

namespace Iridis
{   
    namespace CommandLine
    {
        int Help(int argc, char* argv[]);
        int New(int argc, char* argv[]);
        int Build(int argc, char* argv[]);
        int Run(int argc, char* argv[]);
        int Compile(int argc, char* argv[]);
        int GenBind(int argc, char* argv[]);
    };
};