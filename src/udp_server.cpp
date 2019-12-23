//
// Created by przemek on 22.12.2019.
//

#include "udp_server.h"
#include <boost/bind.hpp>

using namespace boost::asio;

simpleP2P::Udp_Server::Udp_Server(io_service &io_service, Uint16 port)
        : socket(io_service, ip::udp::endpoint(ip::udp::v4(), port)) {
    socket.set_option(ip::udp::socket::reuse_address(true));
    do_receive();
}

void simpleP2P::Udp_Server::handle_receive(const boost::system::error_code &error, std::size_t) {
    if (!error || error == error::message_size) {
        //TODO
        do_receive();
    }
}

void simpleP2P::Udp_Server::do_receive() {
    socket.async_receive_from(
            boost::asio::buffer(recv_buffer), remote_endpoint,
            boost::bind(&Udp_Server::handle_receive, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
}

simpleP2P::Udp_Server::~Udp_Server() {
    socket.close();
}
