#ifndef SIMPLE_P2P_CLI_H
#define SIMPLE_P2P_CLI_H

#include <vector>

#include "GeneralTypes.h"
#include "CLICommand.h"

namespace simpleP2P
{

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

} // namespace simpleP2P
#endif // SIMPLE_P2P_CLI_H
