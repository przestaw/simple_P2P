//
// Created by przemek on 22.12.2019.
//

#ifndef SIMPLE_P2P_UDP_CLIENT_H
#define SIMPLE_P2P_UDP_CLIENT_H

#include <boost/asio/ip/udp.hpp>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <deque>
#include "GeneralTypes.h"
#include "Resource.h"
#include "ResourceDatabase.h"
#include "LoggingModule.h"

namespace simpleP2P {
/**
 * class UDP Client to handle all outgoing packets
 */
class UdpClient : public boost::enable_shared_from_this<UdpClient> {
public:
  /**
   * Constructor of UDP Client
   * @param io_service asio Io Service
   * @param database Database
   * @param logger Logger
   * @param broadcast_address address on which packets will be sent
   * @param broadcast_port port on which packets will be sent
   * @param timeout beacon interval
   */
  UdpClient(boost::asio::io_service &io_service,
            ResourceDatabase &database, LoggingModule &logger,
            const boost::asio::ip::address &broadcast_address,
            Uint16 broadcast_port, Uint32 timeout = 5 * 60);

  /**
   * Destructor closes socket
   */
  ~UdpClient();

  /**
   * Constructs revoke header sends it
   * @param resource Resource to be revoked
   */
  void revoke_file(Resource resource);

private:
  /**
   * Beacon callback
   */
  void fire_beacon();

  /**
   * Put arbitrary datagram in a tx_queue_.
   * @param packet datagram to be put in the tx_queue_
   */
  void send(const std::vector<Uint8> &packet);

  /**
   * Sends the packet from tx_queue_ head
   */
  void transmit();

  /**
   * The function called whenever a write event is received.
   * @param error error code
   * @param bytes_transferred number of transfered bytes
   */
  void write_handler(boost::system::error_code const &error,
                     size_t bytes_transferred);

  /**
   * Static callback function. It ensures that the object still exists
   * and the event is valid before calling the write handler.
   * @param ptr weak ptr to this class
   * @param error error code
   * @param bytes_transferred number of transfered bytes
   */
  static void write_callback(boost::weak_ptr<UdpClient> ptr,
                             boost::system::error_code const &error,
                             size_t bytes_transferred);

  boost::asio::ip::udp::endpoint endpoint_;    //!< Endpoint where data will be sent
  boost::asio::ip::udp::socket socket_;        //!< Socket on which operates Client
  std::deque<std::vector<Uint8> > tx_queue_;   //!< Queue of datagrams to be sent
  boost::asio::deadline_timer timer;           //!< Timer for the beacon
  ResourceDatabase &database;                 //!< Connection to ResourceDatabase
  LoggingModule &logger;                      //!< Connection to LoggingModule
  Uint32 timeout;                              //!< Beacon interval
};
}

#endif //SIMPLE_P2P_UDP_CLIENT_H
