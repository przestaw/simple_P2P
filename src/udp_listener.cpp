//
// Created by przemek on 05.12.2019.
//

#include "udp_listener.h"

namespace simpleP2P {
    UDP_Listener::UDP_Listener(boost::asio::ip::udp::socket socket_c)
            : socket(std::move(socket_c)) {}

    std::thread UDP_Listener::init() {
        return std::thread([=] { worker(); });
    }

    void UDP_Listener::has_changed() {

    }

    void UDP_Listener::worker() {

    }
}