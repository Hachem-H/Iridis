#pragma once

namespace Iridis
{   
    namespace CommandLine
    {
        struct Command
        {
            virtual ~Command() = default;
            virtual int execute(int argc, char* argv[]) { return 0; };
        };

        struct HelpCommand
            : public Command
        {
            int execute(int argc, char* argv[]) override;
        };

        class NewCommand
            : public Command
        {
        private:
            bool IsValidProjectName(const std::string& name);
        public:
            int execute(int argc, char* argv[]) override;
        };

        class BuildCommand
            : public Command
        {
        };

        class RunCommand
            : public Command
        {
        };

        class CompileCommand
            : public Command
        {
        };

        class GenBindCommand
            : public Command
        {
        };
    };
};