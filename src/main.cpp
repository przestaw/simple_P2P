#include "CLI.h"
#include <GeneralTypes.h>
#include <iostream>
#include <logging_module.h>
#include <thread>
#include <random>
#include <boost/program_options.hpp>
#include <boost/asio.hpp>


using namespace simpleP2P;
using namespace boost::program_options;
using namespace boost::asio;

void test_worker(Int16 number){
    std::random_device random_device; //system specific random source

    for(Uint16 iter = 0; iter < 10; ++iter){
        Logging_Module::add_log_line("Message form " + std::to_string(number) + " number " + std::to_string(iter),
                                     std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
        std::this_thread::sleep_for(std::chrono::milliseconds(50 + random_device() % 350));
    }
}

int main(int argc, const char *argv[]) {
    try {
        options_description desc{"Options"};
        desc.add_options()
                ("help,h", "Help screen")
                ("incoming,i", value<int>()->default_value(16), "Incoming connections limit")
                ("file_connections,f", value<int>()->default_value(6), "Connections per downloaded file limit")
                ("logs_file", value<std::string>(), "Log output filename, leave blank to output to stderr");

        variables_map vm;
        store(parse_command_line(argc, argv, desc), vm);
        notify(vm);

        if (vm.count("help"))
            std::cout << desc << '\n';
        else {
            if (vm.count("incoming"))
                std::cout << "incoming: " << vm["incoming"].as<int>() << '\n';
            if (vm.count("file_connections"))
                std::cout << "file_connections: " << vm["file_connections"].as<int>() << '\n';
            if (vm.count("logs_file"))
                std::cout << "logs_file: " << vm["logs_file"].as<std::string>() << '\n';
        }

        std::thread basic[4];

        basic[3] = std::thread(&Logging_Module::logging_thread, std::ref(std::cout));

        for (Uint16 iter = 0; iter < 3; ++iter) {
            basic[iter] = std::thread(test_worker, iter);
        }

        // for (auto &iter : basic) {
        //     iter.join();
        // }

        CLI* commandline = new CLI();
        commandline->init();

        std::cout << "DZIALAM2";

        ip::udp::socket socket_udp();
    }
    catch (const boost::program_options::error &ex) {
        std::cerr << ex.what() << '\n';
    }
}
