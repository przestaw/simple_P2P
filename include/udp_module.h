//
// Created by przemek on 22.12.2019.
//

#ifndef SIMPLE_P2P_UDP_MODULE_H
#define SIMPLE_P2P_UDP_MODULE_H

#include <thread>
#include "GeneralTypes.h"
#include "udp_client.h"
#include "udp_server.h"
#include "logging_module.h"
#include "resource_database.h"

namespace simpleP2P {
    /**
     * Class containing all UDP related resources and logic
     */
    class Udp_Module {
    public:
        /**
         * Constructor
         * @param broadcast_address address on which packets will be sent
         * @param port port on which packets will be sent
         * @param beacon_interval beacon interval
         */
        Udp_Module(Resource_Database &database_c,
                   Logging_Module &logger_c,
                   boost::asio::ip::address broadcast_address,
                   Uint16 port, Uint32 beacon_interval);

        /**
         * Init methods run worker in thread and returns it
         * @return logging thread
         */
        std::thread init();

        /**
         * Sends revoke datagram
         * @param resource Resource t obe revoked
         */
        void revoke_file(const Resource &resource);

    private:
        void run_server();

        boost::shared_ptr<Udp_Client> ptr_client;
        boost::shared_ptr<Udp_Server> ptr_server;
        boost::asio::ip::address broadcast_address;
        Uint16 port;
        Uint32 beacon_interval;
        Resource_Database &database;
        Logging_Module &logger;
    };
}


#endif //SIMPLE_P2P_UDP_MODULE_H
