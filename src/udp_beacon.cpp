//
// Created by przemek on 04.12.2019.
//

#include "udp_beacon.h"

namespace simpleP2P {
    UDP_Beacon(boost::asio::ip::udp::socket
    socket) :
    socket(socket)
            {};

    std::thread UDP_Beacon::init() {
        return std::thread([=] { worker(); });
    }
}