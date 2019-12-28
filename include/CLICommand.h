#ifndef SIMPLE_P2P_CLICOMMAND_H
#define SIMPLE_P2P_CLICOMMAND_H

#include "GeneralTypes.h"
#include <string>
#include <functional>

namespace simpleP2P
{

class CLICommand
{
    std::string name;
    std::string description;
    std::function<Int32(char *[])> function;

public:
    CLICommand(std::string, std::string, std::function<Int32(char *[])>);
    ~CLICommand(){};
    void operator()(std::string) const;
    std::string getName() const { return name;};
};

} // namespace simpleP2P

#endif // SIMPLE_P2P_CLICOMMAND_H