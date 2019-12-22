//
// Created by przemek on 22.12.2019.
//

#include <boost/asio/io_service.hpp>
#include "udp_module.h"
#include "udp_server.h"
#include "udp_client.h"

std::thread simpleP2P::Udp_Module::init() {
    return std::thread([=] { run_server(port); });
}

void simpleP2P::Udp_Module::run_server(Uint16 port) {
    try {
        boost::asio::io_service io_service;
        Udp_Server server(io_service, port);
        Udp_Client client(io_service, port, 60);
        io_service.run();
    } catch (std::exception &e) {
        //TODO : log :  e.what();
    }
}
