
#include <DownloadService.h>
#include <FileManager.h>

#include "CLI.h"
#include <GeneralTypes.h>
#include <iostream>
#include <logging_module.h>
#include <thread>
#include <boost/program_options.hpp>
#include <boost/asio.hpp>
#include <udp_server.h>
#include <udp_client.h>
#include <udp_module.h>

using namespace simpleP2P;
using namespace boost;

int main(int argc, const char *argv[]) {

    //TODO: Parameters variables
    try {
        program_options::options_description desc{"Options"};
        desc.add_options()
                ("help,h",
                 "Help screen")
                ("incoming,i", program_options::value<int>()->default_value(16),
                 "Incoming connections limit")
                ("file_connections,f", program_options::value<int>()->default_value(6),
                 "Connections per downloaded file limit")
                ("logs_file", program_options::value<std::string>(),
                 "Log output filename, leave blank to output to stderr");

        program_options::variables_map vm;
        store(parse_command_line(argc, argv, desc), vm);
        notify(vm);

        if (vm.count("help"))
            std::cout << desc << '\n';
        else {
            // TODO : save parameters
            if (vm.count("incoming"))
                std::cout << "incoming: " << vm["incoming"].as<int>() << '\n';
            if (vm.count("file_connections"))
                std::cout << "file_connections: " << vm["file_connections"].as<int>() << '\n';
            if (vm.count("logs_file"))
                std::cout << "logs_file: " << vm["logs_file"].as<std::string>() << '\n';
        }
    }
    catch (const boost::program_options::error &ex) {
        std::cerr << ex.what() << '\n';
    }

    std::thread basic[4];
    /*
     * Create threads for all modules and connect them e.g. by signal-slot
     */
    //TODO: use parameters
    Host localhost(boost::asio::ip::address::from_string("192.168.1.1"));
    Logging_Module logger; //TODO file OR default = std::cerr
    Resource_Database database(localhost); //TODO ADRR
    Udp_Module udp(database, logger, boost::asio::ip::address::from_string(BROADCAST_ADDRESS), BROADCAST_PORT,
                   10); // basic test

    {
        Resource res = Resource("Bananowe jointy", 102070);
        database.add_file(res);

        res = Resource("XD", 10302000);

        database.add_file(res);
        res = Resource("Karmiace kaczki", 1910);

        database.add_file(res);
    }

    boost::asio::io_service io_service;
    FileManager fm;
    // ^ XDD

    //TODO : localhost as shared_ptr
    Printer printer(std::cout);
    CLI commandline(database, logger, io_service, fm, *database.getHost().get(), printer);

    basic[0] = logger.init();
    basic[1] = udp.init();
    basic[2] = printer.init();
    basic[3] = commandline.init();
    std::cout << "dupa";

    for (auto &iter : basic) {
        iter.join();
    }
}
