//
// Created by przemek on 22.12.2019.
//

#include "udp_server.h"
#include <boost/bind.hpp>
#include <iostream>

using namespace boost::asio;

simpleP2P::Udp_Server::Udp_Server(io_service &io_service, const boost::asio::ip::address &broadcast_address,
                                  Uint16 broadcast_port)
        : socket_(io_service, ip::udp::endpoint(broadcast_address, broadcast_port)),
          remote_endpoint() {
    socket_.set_option(
            ip::udp::socket::reuse_address(true));

    do_receive();
}

void simpleP2P::Udp_Server::handle_receive(const boost::system::error_code &error, size_t bytes_transferred) {
    if (!error || error == error::message_size) {
        //TODO
        std::cerr << recv_buffer.data() << "\n ^ receiver -> remote: "
                  << remote_endpoint.address().to_string()
                  << " received : " << bytes_transferred << " bytes\n";
        do_receive();
    }
}

void simpleP2P::Udp_Server::do_receive() {
    socket_.async_receive_from(
            boost::asio::buffer(recv_buffer), remote_endpoint,
            boost::bind(&Udp_Server::handle_receive, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
}

simpleP2P::Udp_Server::~Udp_Server() {
    socket_.close();
}
