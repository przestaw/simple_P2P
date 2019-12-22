#include "CLI.h"
#include <iostream>
#include <boost/tokenizer.hpp>

namespace simpleP2P
{
CLI::CLI()
{
    CLICommands = {
        CLICommand("help", "", nullptr),
        CLICommand("add", "", nullptr),
        /*
            spawn thread
            create object resource
            add task to add resource to sync buffer
            thread join
        */
        CLICommand("remove", "", nullptr),
        /*
            spawn thread
            add task to remove resource to sync buffer
            join
        */
        CLICommand("revoke", "", nullptr),
        /* 
            spawn thread
            send via udp
            join
        */
        CLICommand("download", "", nullptr)
        /*
            spawn DownloadWorker
            elo
        */

    };
}

void CLI::init()
{

    std::string line;
    while (std::getline(std::cin, line))
    {
        // std::stringstream line_stream{line};
        // std::vector<string> tokens;
        // std::string tmp;
        // while (getline(check1, intermediate, ' '))
        // {
        //     tokens.push_back(intermediate);
        // }
        boost::tokenizer<> tokens{line};
        for (const auto &t : tokens)
        {
            std::cout << t << '\n';
        }
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

} // namespace simpleP2P