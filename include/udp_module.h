//
// Created by przemek on 22.12.2019.
//

#ifndef SIMPLE_P2P_UDP_MODULE_H
#define SIMPLE_P2P_UDP_MODULE_H

#include <thread>
#include "GeneralTypes.h"
#include "udp_client.h"
#include "udp_server.h"

namespace simpleP2P {
    class Udp_Module {
    public:
        Udp_Module(Uint16 port);

        std::thread init();

    private:
        void run_server();

        Uint16 port;
    };
}


#endif //SIMPLE_P2P_UDP_MODULE_H
