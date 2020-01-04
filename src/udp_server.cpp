//
// Created by przemek on 22.12.2019.
//

#include "udp_server.h"
#include <boost/bind.hpp>
#include <iostream>

#include <iomanip>

using namespace boost::asio;

simpleP2P::Udp_Server::Udp_Server(io_service &io_service,
                                  Resource_Database &database_c, Logging_Module &logger_c,
                                  const boost::asio::ip::address &broadcast_address,
                                  Uint16 broadcast_port)
        : socket_(io_service, ip::udp::endpoint(broadcast_address, broadcast_port)),
          remote_endpoint(), recv_buffer(), database(database_c), logger(logger_c) {
    socket_.set_option(
            ip::udp::socket::reuse_address(true));

    do_receive();
}

void simpleP2P::Udp_Server::handle_receive(const boost::system::error_code &error, size_t bytes_transferred) {
    if (!error || error == error::message_size) {
        //TODO negate to switch branches
        if(*database.getHost().get() == Host(remote_endpoint.address())){
            Uint8 *buf = recv_buffer.data() + 1;
            if(recv_buffer.front() == FILE_LIST){
                logger.add_log_line("received (files beacon) : " +
                                    std::to_string(bytes_transferred) +
                                    " bytes over UDP\n from : " +
                                    remote_endpoint.address().to_string(),
                                    std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
                Uint64 iter = be64toh(*(reinterpret_cast<Uint64 *>(buf)));
                buf += 8; //move to first resource
                for (Uint64 i = 0; i < iter; ++i) {
                    Uint8 *end = std::find(buf, buf + 256, '\0');
                    std::string filename(buf, end);

                    Resource res = Resource(filename,
                                            be64toh(
                                                    *(reinterpret_cast<Uint64 *>(
                                                            buf + 256))));
                    buf += 264;//move to next header
                    //TODO: add to host
                    //TODO: update HOST in database
                }
            } else if (recv_buffer.front() == REVOKE) {
                logger.add_log_line("received (revocation) : " +
                                    std::to_string(bytes_transferred) +
                                    " bytes over UDP\n from : " +
                                    remote_endpoint.address().to_string(),
                                    std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
                Uint8 *end = std::find(buf, buf + 256, '\0');
                std::string filename(buf, end);

                Resource res = Resource(filename,
                                        be64toh(
                                                *(reinterpret_cast<Uint64 *>(
                                                        buf + 256))));
                //TODO: revoke res
            }
        }
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
