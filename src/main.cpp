
#include <GeneralTypes.h>
#include <iostream>
#include <logging_module.h>
#include <thread>
#include <random>
#include <boost/program_options.hpp>
#include <boost/asio.hpp>

using namespace simpleP2P;
using namespace boost;

int main(int argc, const char *argv[]) {
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
            if (vm.count("incoming"))
                std::cout << "incoming: " << vm["incoming"].as<int>() << '\n';
            if (vm.count("file_connections"))
                std::cout << "file_connections: " << vm["file_connections"].as<int>() << '\n';
            if (vm.count("logs_file"))
                std::cout << "logs_file: " << vm["logs_file"].as<std::string>() << '\n';
        }

        std::thread basic[4];

        /*
         * Create threads for all modules and connect them e.g. by signal-slot
         */
        Logging_Module logger;
        basic[3] = logger.init();

//        for (Uint16 iter = 0; iter < 3; ++iter) {
//            basic[iter] = std::thread(test_worker, iter);
//        }
//
        for (auto &iter : basic) {
            iter.join();
        }

//        ip::udp::socket socket_udp();
    }
    catch (const boost::program_options::error &ex) {
        std::cerr << ex.what() << '\n';
    }
}
