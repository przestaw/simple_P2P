//
// Created by przemek on 09.12.2019.
//

#ifndef SIMPLE_P2P_HOST_H
#define SIMPLE_P2P_HOST_H

#include <chrono>
#include <string>
#include <vector>

#include <GeneralTypes.h>
#include <boost/asio.hpp>

namespace simpleP2P {

class Resource; //!< Forward declaration

/**
 * Class contains node information and points to files it possess
 */
class Host {
public:
  /**
   * Constructor
   * @param ip Ip of the Host
   */
  Host(boost::asio::ip::address ip);

  /**
   * Determines if host has resource
   * @param res Resource to be checked
   * @return true if Host has Resource res
   */
  bool has_resource(Resource res);

  /**
   * Operator == checks host_ip for equality
   * @param other other
   * @return true if equal
   */
  bool operator==(const Host &other) const;

  /**
   * Operator != checks host_ip for equality
   * @param other other
   * @return true if not equal
   */
  bool operator!=(const Host &other) const;

  boost::asio::ip::tcp::endpoint get_endpoint() const;
  // Int16 get_port() const;
  // boost::asio::ip::address get_address() const;
  bool is_retarded() const;
  void increase_timeout_counter();
  std::chrono::system_clock::time_point get_ban_time_point() const;

private:
  boost::asio::ip::address host_ip; //!< Ip of the Host

  /*atrribs not checked for equality*/
  // TODO: timeout etc stats
  std::vector<Resource *>
      possesed_resources; //!< Resources possessed by the Host
  Uint16 port;

  Int8 timeout_counter;
  Int32 ban_time_point;
  friend class Resource_Database; //!< friendship to manage Host's Resources
                                  //!< timeouts etc
};
} // namespace simpleP2P

#endif // SIMPLE_P2P_HOST_H
