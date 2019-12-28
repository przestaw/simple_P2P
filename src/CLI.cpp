#include "CLI.h"
#include "CLICommand.h"
#include <string>
#include <iostream>
#include <boost/tokenizer.hpp>
#include <GeneralTypes.h>
#include <functional>

namespace simpleP2P
{
CLI::CLI()
{
    CLICommands = {
        CLICommand("help", "prints available commands", [this](std::string dupa){for(auto &command : CLICommands) std::cout << command; return 69;}),
        CLICommand("add", "\"add name_of_file\" - adds local file to resource db, will be broadcasted", nullptr),
        /*
            spawn thread
            create object resource
            add task to add resource to sync buffer
            thread join
        */
        CLICommand("remove", "\"remove name_of_file\" - removes a resource", nullptr),
        /*
            spawn thread
            add task to remove resource to sync buffer
            join
        */
        CLICommand("revoke", "\"revoke name_of_file\" - make sall instances of a file undownloadable", nullptr),
        /* 
            spawn thread
            send via udp
            join
        */
        CLICommand("download", "\"download name_of_file\" - downloads a file using p2p", nullptr),
        /*
            spawn DownloadWorker
            elo
        */
        CLICommand("local", "prints files in resource database", nullptr),
        /*
            
        */
        CLICommand("global", "prints files available to be download", nullptr),
        /*
            
        */
        CLICommand("quit", "leaves the program", nullptr)
        /*
            
        */
    };
}

void CLI::init()
{
    std::string line;
    std::vector<std::string> vec;
    while (std::getline(std::cin, line))
    {
        boost::tokenizer<> tokens{line};
        vec.assign(tokens.begin(), tokens.end());
        if (vec.size() == 0)
            continue;
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
    for (auto command : CLICommands)
    {
        std::cout << command.getName() << "\n";

        if (name == command.getName())
        {
            command(arg);
            std::cout << " coś robimy";
        }
    }
}

CLI::~CLI() {}

} // namespace simpleP2P