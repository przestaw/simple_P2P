//
// Created by przemek on 22.12.2019.
//

#include <boost/asio/io_service.hpp>
#include <utility>
#include "UdpModule.h"
#include "UdpServer.h"
#include "UdpClient.h"

using namespace boost::asio;

simpleP2P::UdpModule::UdpModule(ResourceDatabase &database_c,
                                Logging_Module &logger_c,
                                ip::address broadcast_address_c,
                                Uint16 port_c, Uint32 beacon_interval_c)
    : ptr_client(nullptr), ptr_server(nullptr),
      broadcast_address(std::move(broadcast_address_c)),
      port(port_c), beacon_interval(beacon_interval_c),
      database(database_c), logger(logger_c) {}

std::thread simpleP2P::UdpModule::init() {
  return std::thread([this] { this->run_server(); });
}

void simpleP2P::UdpModule::run_server() {
  try {
    boost::asio::io_service io_service;
    UdpClient client(io_service,
                     database,
                     logger,
                     broadcast_address,
                     port,
                     beacon_interval);
    ptr_client = boost::shared_ptr<UdpClient>(&client);
    UdpServer server(io_service,
                     database,
                     logger,
                     broadcast_address,
                     port);
    ptr_server = boost::shared_ptr<UdpServer>(&server);
    io_service.run();
  } catch (std::exception &e) {
    logger.add_log_line(
        e.what(),
        std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now()));
  }
}

bool simpleP2P::UdpModule::revoke_file(const simpleP2P::Resource &resource) {
  if (ptr_client.get() != nullptr) {
    if (database.get_localhost()->has_resource(resource)) {
      ptr_client->revoke_file(resource);
      database.revoke_resource(resource);
      return true;
    }
  }
  return false;
}
