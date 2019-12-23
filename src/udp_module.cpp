//
// Created by przemek on 22.12.2019.
//

#include <boost/asio/io_service.hpp>
#include <utility>
#include "udp_module.h"
#include "udp_server.h"
#include "udp_client.h"

using namespace boost::asio;

simpleP2P::Udp_Module::Udp_Module(ip::address broadcast_address_c,
                                  Uint16 port_c, Uint32 beacon_interval_c)
        : ptr_client(nullptr), ptr_server(nullptr),
          broadcast_address(std::move(broadcast_address_c)),
          port(port_c), beacon_interval(beacon_interval_c) {}

std::thread simpleP2P::Udp_Module::init() {
    return std::thread([this] { this->run_server(); });
}

void simpleP2P::Udp_Module::run_server() {
    try {
        boost::asio::io_service io_service;
        Udp_Client client(io_service,
                          broadcast_address,
                          port,
                          beacon_interval);
        ptr_client = boost::shared_ptr<Udp_Client>(&client);
        Udp_Server server(io_service,
                          broadcast_address,
                          port);
        ptr_server = boost::shared_ptr<Udp_Server>(&server);
        io_service.run();
    } catch (std::exception &e) {
        //TODO : log :  e.what();
    }
}

void simpleP2P::Udp_Module::revoke_file(const simpleP2P::Resource &resource) {
    if (ptr_client.get() != nullptr) {
        ptr_client->revoke_file(resource);
    }
}
