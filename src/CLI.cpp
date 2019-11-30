#include "CLI.h"

CLI::CLI()
{
    CLICommands = {
        CLICommand("help", "", nullptr),
        CLICommand("add", "", nullptr),
        CLICommand("remove", "", nullptr),
        CLICommand("revoke", "", nullptr),
        CLICommand("download", "", nullptr)};
}

void CLI::init()
{
    for (;;)
    {
        // wait for input
        // parse
        // execute
        }
}

void CLI::execute_command(std::string name, char *args[])
{
    for (auto command : CLICommands)
    {
        if (name == command.getName())
        {
            command(args);
        }
    }
}
