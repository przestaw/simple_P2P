//
// Created by przemek on 04.12.2019.
//

#include "udp_beacon.h"

namespace simpleP2P {
    UDP_Beacon::UDP_Beacon(boost::asio::ip::udp::socket socket_c)
            : socket(std::move(socket)) {}

    std::thread UDP_Beacon::init() {
        return std::thread([=] { worker(); });
    }

    void UDP_Beacon::revoke_file(std::string resource) {

    }

    void UDP_Beacon::fire_beacon() {

    }

    void UDP_Beacon::worker() {

    }
}