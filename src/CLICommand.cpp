#ifndef SIMPLE_P2P_CLI_H
#define SIMPLE_P2P_CLI_H

#include "CLICommand.h"
#include <string>
#include <iostream>

namespace simpleP2P
{
    CLICommand::CLICommand(std::string name_, std::string description_, std::function<Int32(char *[])> function_)
    {
        name = name_; 
        description = description_;
        function = function_;
    };

    void CLICommand::operator()(std::string) const
    {

    };


} // namespace simpleP2P


#endif // SIMPLE_P2P_CLI_H