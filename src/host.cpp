//
// Created by przemek on 09.12.2019.
//

#include "host.h"
#include <utility>
#include "resource.h"
#include <iostream>
#include <utility>

namespace simpleP2P {
    Host::Host(boost::asio::ip::address ip)
            : host_ip(std::move(ip)) {}

    bool Host::has_resource(Resource res) {
        return std::count_if(possesed_resources.begin(),
                             possesed_resources.end(),
                             [&res](std::weak_ptr<Resource> &it) {
                                 return res == *(it.lock().get());
                             }) != 0;
    }

    bool Host::operator==(const Host &other) const {
        return this->host_ip == other.host_ip;
    }

    bool Host::operator!=(const Host &other) const {
        return this->host_ip != other.host_ip;
    }

    void Host::remove_resource(std::shared_ptr<Resource> res) {
        possesed_resources.erase(
                std::remove_if(
                        possesed_resources.begin(),
                        possesed_resources.end(),
                        [&res](auto &it) {
                            return it.lock() == res;
                        }
                ), possesed_resources.end());
    }

// boost::asio::ip::address Host::get_address() const { return host_ip; }

// Int16 Host::get_port() const { return port; }

bool Host::is_retarded() {
  // return ban_time >
  //        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  return false;
}

void Host::increase_timeout_counter() {
  // TODO possible races
  // timeout_counter++;

  // Int8 &&limit = TIMEOUT_COUNTER_LIMIT;

  // if (timeout_counter.compare_exchange_strong(limit, 0)) {
  //   using namespace std::literals;
  //   ban_time = std::chrono::system_clock::to_time_t(
  //       std::chrono::system_clock::now() + BAN_TIME);
  // }
}

std::chrono::system_clock::time_point Host::get_ban_time_point() const {
  // return std::chrono::system_clock::from_time_t(ban_time);
  return std::chrono::system_clock::now();
}

boost::asio::ip::tcp::endpoint Host::get_endpoint() const {
  return boost::asio::ip::tcp::endpoint{host_ip, port};
}

} // namespace simpleP2P