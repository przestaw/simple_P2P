#ifndef SIMPLE_P2P_CLI_H
#define SIMPLE_P2P_CLI_H

#include <vector>

#include "GeneralTypes.h"
#include "DownloadService.h"
#include "CLICommand.h"
#include "resource_database.h"
#include "resource.h"
#include "logging_module.h"
#include "FileManager.h"
#include "printer.h"
#include <sstream>

namespace simpleP2P {

    class CLI {
        std::vector<CLICommand> CLICommands;
        Resource_Database &res_db;
        Logging_Module &Logger;
        boost::asio::io_service &io_service;
        FileManager &fm;
        Host &localhost;
        Printer &printer;
        std::stringstream stream;

        void print_help() const;

        void execute_command(std::string name, std::string arg);

        void print_init_info();

        void print_text(std::stringstream &text);

        void init2();

    public:
        CLI(Resource_Database &res_db_, Logging_Module &Logger_, boost::asio::io_service &io_service_, FileManager &fm_,
            Host &localhost_, Printer &printer_);

        ~CLI();

        std::thread init();

    };

} // namespace simpleP2P
#endif // SIMPLE_P2P_CLI_H