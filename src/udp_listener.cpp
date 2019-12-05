//
// Created by przemek on 05.12.2019.
//

#include "udp_listener.h"

namespace simpleP2P {
    UDP_Listener::UDP_Listener(boost::asio::ip::udp::socket socket) : socket(socket) {}
}