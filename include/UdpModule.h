//
// Created by przemek on 22.12.2019.
//

#ifndef SIMPLE_P2P_UDP_MODULE_H
#define SIMPLE_P2P_UDP_MODULE_H

#include <thread>
#include "GeneralTypes.h"
#include "UdpClient.h"
#include "UdpServer.h"
#include "LoggingModule.h"
#include "ResourceDatabase.h"

namespace simpleP2P {
/**
 * Class containing all UDP related resources and logic
 */
class UdpModule {
public:
  /**
   * Constructor
   * @param broadcast_address address on which packets will be sent
   * @param port port on which packets will be sent
   * @param beacon_interval beacon interval
   */
  UdpModule(ResourceDatabase &database_c,
            LoggingModule &logger_c,
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
   * @return true on success
   */
  bool revoke_file(const Resource &resource);

private:
  /**
   * Internal thread function. Initialise Client, Server and their IO context
   */
  void run_server();

  boost::shared_ptr<UdpClient> ptr_client;            //!< Handle to UDP Client
  boost::shared_ptr<UdpServer> ptr_server;            //!< Handle to UDP Server
  boost::asio::ip::address broadcast_address;          //!< UDP broadcast address
  Uint16 port;                                         //!< UDP broadcast port
  Uint32 beacon_interval;                              //!< Beacon interval
  ResourceDatabase &database;                         //!< Connection to ResourceDatabase
  LoggingModule &logger;                              //!< Connection to LoggingModule
};
}

#endif //SIMPLE_P2P_UDP_MODULE_H
