#ifndef SIMPLE_P2P_CLI_H
#define SIMPLE_P2P_CLI_H

#include "CLICommand.h"
#include <string>
#include <iostream>
#include <iomanip>

namespace simpleP2P {
    CLICommand::CLICommand(std::string name_, std::string description_,
                           std::function<Int32(const std::string &)> function_) {
        name = name_;
        description = description_;
        function = function_;
    };

    void CLICommand::operator()(std::string argument) const {
        if (function != nullptr)
            function(argument);
        else
            std::cout << "not implemented yet :(\n";
    };

    std::ostream &operator<<(std::ostream &os, const CLICommand &command) {
        os << "- " << std::setw(10) << std::left << command.getName() << " " << command.getDesc() << '\n';
        return os;
    }

} // namespace simpleP2P

#endif // SIMPLE_P2P_CLI_H