#ifndef __CLICOMMAND_H__
#define __CLICOMMAND_H__

#include "GeneralTypes.h"
#include <string>
#include <functional>

class CLICommand
{
    std::string name;
    std::string description;
    std::function<Int32(char *[])> function;

public:
    CLICommand(std::string name, std::string description, std::function<Int32(char *[])> function);
    ~CLICommand();
    void operator()(char *[]) const;
    std::string getName() const;
};

#endif // __CLICOMMAND_H__