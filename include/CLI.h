#ifndef __CLI_H__
#define __CLI_H__

#include <vector>

#include "GeneralTypes.h"
#include "CLICommand.h"

class CLI
{
    std::vector<CLICommand> CLICommands;
    void print_help() const;
    void execute_command(std::string name, char *args[]);

public:
    CLI();
    ~CLI();
    void init();
};

#endif // __CLI_H__
