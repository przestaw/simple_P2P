#include "CLI.h"
#include "CLICommand.h"
#include <string>
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
        CLICommand("download", "", nullptr),
        /*
            spawn DownloadWorker
            elo
        */
        CLICommand("local", "", nullptr),
        /*
            
        */
        CLICommand("global", "", nullptr),
        /*
            
        */
        CLICommand("quit", "", nullptr)
        /*
            
        */
    };
}

void CLI::init()
{

    std::cout << "DZIALAM";

    std::string line;
    std::vector<std::string> vec;
    while (std::getline(std::cin, line))
    {
        boost::tokenizer<> tokens{line};
        vec.assign(tokens.begin(), tokens.end());
        for (const auto &t : vec)
        {
            std::cout << t << '\n';
        }
        std::cout << "---- " << vec[0] << "\n";

        if (vec.size() == 1)
            vec.push_back("");

        execute_command(vec[0], vec[1]);
    }
}

void CLI::execute_command(std::string name, std::string arg)
{
    std::cout << "test1" << name << "\n";
    for (auto command : CLICommands)
    {
        std::cout << command.getName() << "\n";

        if (name == command.getName())
        {
            //command(arg);
            std::cout << "coś robimy";
        }
    }
}

CLI::~CLI() {}

} // namespace simpleP2P