//
// Created by przemek on 22.12.2019.
//

#ifndef SIMPLE_P2P_UDP_SERVER_H
#define SIMPLE_P2P_UDP_SERVER_H

#include <boost/asio/ip/udp.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "GeneralTypes.h"

#define buffer_size 1500

namespace simpleP2P {
    class Udp_Server {
    public:
        Udp_Server(boost::asio::io_service &io_service, Uint16 port);

        ~Udp_Server();

    private:
        void do_receive();

        void handle_receive(const boost::system::error_code &error,
                            std::size_t);

        boost::asio::ip::udp::socket socket;
        boost::asio::ip::udp::endpoint remote_endpoint;
        boost::array<Int8, buffer_size> recv_buffer;
    };
}

#endif //SIMPLE_P2P_UDP_SERVER_H
