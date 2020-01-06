//
// Created by przemek on 05.12.2019.
//

#ifndef SIMPLE_P2P_UDP_LISTENER_H
#define SIMPLE_P2P_UDP_LISTENER_H

#include <vector>
#include <string>
#include <boost/asio.hpp>
#include <thread>
#include "resource_database.h"

namespace simpleP2P {

    class UDP_Listener {
    public:
        UDP_Listener(boost::asio::ip::udp::socket socket_c);

        std::thread init();

    private:
        void worker();

        void has_changed();                     // if data about files is stored elsewhere
        boost::asio::ip::udp::socket socket;     //!< socket on which listens for file information

        //Resource_Database database;
    };
}

#endif //SIMPLE_P2P_UDP_LISTENER_H
