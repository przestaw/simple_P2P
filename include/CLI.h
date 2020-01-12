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

        /**
         * Function for printing through CLI and clearing stringstream
         * @param text Stringstream with text to be printed
         */
        void print_text(std::stringstream &text);
        void print_help() const;
        void execute_command(std::string name, std::string arg);
        void print_init_info();
        void start_CLI();

    public:
        /**
         * @brief Construct a new Download Service object
         *
         * @param res_db_
         * @param Logger_
         * @param io_service_
         * @param fm_
         * @param localhost_
         * @param printer_
         */
        CLI(Resource_Database &res_db_, Logging_Module &Logger_, boost::asio::io_service &io_service_, FileManager &fm_,
            Host &localhost_, Printer &printer_);

        ~CLI();

        /**
         * @brief Method initiating CLI in the current thread.
         *
         */
        std::thread init();

    };

} // namespace simpleP2P
#endif // SIMPLE_P2P_CLI_H