#include "CLI.h"
#include "DownloadService.h"
#include "CLICommand.h"
#include "resource_database.h"
#include "logging_module.h"
#include "FileManager.h"
#include "resource.h"
#include <string>
#include <iostream>
#include <boost/tokenizer.hpp>
#include <experimental/filesystem>
#include <GeneralTypes.h>
#include <functional>
#include <system_error>

namespace simpleP2P
{
CLI::CLI(Resource_Database &res_db_, Logging_Module &Logger_, boost::asio::io_service &io_service_, FileManager &fm_, Host &localhost_) : res_db(res_db_), Logger(Logger_), io_service(io_service_), fm(fm_), localhost(localhost_)
{
    CLICommands = {
        CLICommand("add", "\"add name_of_file\" - adds local file to resource db, will be broadcasted", [this](std::string name_of_file) {
            std::error_code errc;
            //std::cout << "Current path is " << std::experimental::filesystem::current_path() << '\n';
            Uint64 size = std::experimental::filesystem::file_size(name_of_file, errc);

            if (not errc)
            {
                std::cout << " File size is: " << size << std::endl;

                const Resource new_resource(name_of_file, size, name_of_file); //not sure if path is correct?
                //res_db.add_file(new_resource, boost::asio::ip::address::from_string("192.168.1.1")); 
                res_db.add_file(new_resource); 
                return 1;
            }
            else if (errc == std::errc::no_such_file_or_directory)
            {
                std::cout << " File does not exist. Are you sure it is in Simple_P2P.bin?" << std::endl;
                return 0;
            }
        }),

        CLICommand("remove", "\"remove name_of_file\" - removes a resource", [this](std::string name_of_file) {
            //std::vector<Resource *> resources = res_db.getResources();   --- causes segfault
            std::vector<Resource *> resources;

            for (auto resource : resources)
            {
                if (resource->getName() == name_of_file)
                {
                    // std::thread remover([this](Resource resource){
                    //     res_db.remove_file(resource);
                    // });
                    // remover.detach();

                    return 1;
                }
            }
            return 0;
        }),

        /*
            spawn thread
            add task to remove resource to sync buffer
            join
        */
        CLICommand("revoke", "\"revoke name_of_file\" - makes all instances of a file undownloadable", nullptr),
        /* 
            spawn thread
            send via udp
            join
        */
        CLICommand("download", "\"download name_of_file\" - downloads a file using p2p", [this](std::string name_of_file) {
            //std::vector<Resource *> resources = res_db.getResources();   --- causes segfault
            std::vector<Resource *> resources;

            for (auto resource : resources)
            {
                if (resource->getName() == name_of_file)
                {
                    // std::thread downloader([this](){
                    //     DownloadService ds(Logger, io_service, fm, resource);
                    //     ds.init();
                    // });
                    // downloader.detach();

                    return 1;
                }
            }
            return 0;
        }),

        //CLICommand("local", "prints files in resource database", nullptr),

        CLICommand("global", "prints files available to be downloaded", [this](std::string dupa) {
            std::vector<std::shared_ptr<Resource>> resources = res_db.getResources();

            for (auto resource : resources)
            {
                std::cout << " " << resource->getName() << "\n";
            }
            return 0;
        }),

        CLICommand("help", "prints available commands", [this](std::string dupa) {
            for (auto &command : CLICommands)
                std::cout << command;
            return 69; }), CLICommand("quit", "leaves the program", nullptr)
    };
}

void CLI::print_init_info()
{
    std::cout << "-----\n";
    std::cout << "Simple P2P downloader - TIN 19Z project\n";
    std::cout << "Wiktor Michalski\n";
    std::cout << "Przemyslaw Stawczyk\n";
    std::cout << "Maciej Szulik\n";
    std::cout << "Kamil Zacharczuk\n";
    std::cout << "-----\n";
    execute_command("help", "");
}

void CLI::init()
{
    std::string line;
    std::vector<std::string> vec;

    print_init_info();

    while (std::getline(std::cin, line))
    {
        typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
        boost::char_separator<char> sep(" ");
        tokenizer tokens(line, sep);

        vec.assign(tokens.begin(), tokens.end());
        if (vec.size() == 0)
            continue;
        // for (const auto &t : vec)
        // {
        //     std::cout << t << '\n';
        // }
        //std::cout << "---- " << vec[0] << "\n";

        if (vec.size() == 1)
            vec.push_back("");

        execute_command(vec[0], vec[1]);
    }
}

void CLI::execute_command(std::string name, std::string arg)
{
    bool found = 0;
    for (auto command : CLICommands)
    {
        //std::cout << command.getName() << "\n";

        if (name == command.getName())
        {
            found = 1;
            command(arg);
        }
    }
    if (found == 0)
    {
        std::cout << "* type 'help' for the list of available commands\n";
    }
}

CLI::~CLI() {}

} // namespace simpleP2P