#ifndef SIMPLE_P2P_CLI_H
#define SIMPLE_P2P_CLI_H

#include <vector>

#include "GeneralTypes.h"
#include "CLICommand.h"
#include "resource_database.h"
#include "resource.h"
#include "logging_module.h"
#include "FileManager.h"

namespace simpleP2P
{

class CLI
{
    std::vector<CLICommand> CLICommands;
    Resource_Database& res_db;
    Logging_Module* Logger;
    boost::asio::io_service& io_service;
    FileManager* fm;
    void print_help() const;
    void execute_command(std::string name, std::string arg);
    void print_init_info();

public:
    CLI(Resource_Database& res_db_, Logging_Module* Logger_, boost::asio::io_service& io_service_, FileManager* fm_);
    ~CLI();
    void init();
};

} // namespace simpleP2P
#endif // SIMPLE_P2P_CLI_H
