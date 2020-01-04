
#include <DownloadService.h>
#include <FileManager.h>

#include <GeneralTypes.h>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <logging_module.h>
#include <thread>
#include <udp_client.h>
#include <udp_module.h>
#include <udp_server.h>

using namespace simpleP2P;
using namespace boost;

int main(int argc, const char *argv[]) {
  //   // TODO: Parameters variables
  //   try {
  //     program_options::options_description desc{"Options"};
  //     desc.add_options()("help,h", "Help screen")(
  //         "incoming,i", program_options::value<int>()->default_value(16),
  //         "Incoming connections limit")(
  //         "file_connections,f",
  //         program_options::value<int>()->default_value(6), "Connections per
  //         downloaded file limit")( "logs_file",
  //         program_options::value<std::string>(), "Log output filename, leave
  //         blank to output to stderr");

  //     program_options::variables_map vm;
  //     store(parse_command_line(argc, argv, desc), vm);
  //     notify(vm);

  //     if (vm.count("help"))
  //       std::cout << desc << '\n';
  //     else {
  //       // TODO : save parameters
  //       if (vm.count("incoming"))
  //         std::cout << "incoming: " << vm["incoming"].as<int>() << '\n';
  //       if (vm.count("file_connections"))
  //         std::cout << "file_connections: " <<
  //         vm["file_connections"].as<int>()
  //                   << '\n';
  //       if (vm.count("logs_file"))
  //         std::cout << "logs_file: " << vm["logs_file"].as<std::string>() <<
  //         '\n';
  //     }

  //     std::thread basic[4];

  //     /*
  //      * Create threads for all modules and connect them e.g. by signal-slot
  //      */
  //     Logging_Module logger;
  //     basic[3] = logger.init();

  //     //        for (Uint16 iter = 0; iter < 3; ++iter) {
  //     //            basic[iter] = std::thread(test_worker, iter);
  //     //        }
  //     //
  //     for (auto &iter : basic) {
  //       iter.join();
  //     }

  //     //        ip::udp::socket socket_udp();
  //   } catch (const boost::program_options::error &ex) {
  //     std::cerr << ex.what() << '\n';
  //   }

  //   std::thread basic[4];
  //   /*
  //    * Create threads for all modules and connect them e.g. by signal-slot
  //    */
  //   // TODO: use parameters
  //   Logging_Module logger;
  //   // Udp_Module
  //   udp(boost::asio::ip::address::from_string(BROADCAST_ADDRESS),
  //   // 19000, 5); // basic test

  //   basic[0] = logger.init();
  //   // basic[1] = udp.init();

  //   for (auto &iter : basic) {
  //     iter.join();
  //   }

  std::thread basic[4];
  Logging_Module logging_module;
  FileManager file_manager;
  boost::asio::io_service io_service;
  std::shared_ptr<Resource> resource =
      std::make_shared<Resource>("nazwa", 20000, "");
  Host *host = new Host(boost::asio::ip::address::from_string("192.168.0.1"));
  Host *host2 = new Host(boost::asio::ip::address::from_string("192.168.0.2"));

  resource->add_host(host);
  resource->add_host(host2);

  download::DownloadService download_service{logging_module, io_service,
                                             file_manager, resource};
  basic[0] = logging_module.init();
  basic[1] = download_service.init();
  for (auto &iter : basic) {
    iter.join();
  }
}
