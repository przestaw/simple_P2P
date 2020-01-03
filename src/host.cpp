//
// Created by przemek on 09.12.2019.
//

#include "host.h"
#include "resource.h"
#include <iostream>
#include <utility>

namespace simpleP2P {

Host::Host(boost::asio::ip::address ip) : host_ip(std::move(ip)) {}

bool Host::has_resource(Resource res) {
  return std::count_if(possesed_resources.begin(), possesed_resources.end(),
                       [&res](Resource *it) { return res == *it; }) != 0;
}

bool Host::operator==(const Host &other) const {
  return this->host_ip == other.host_ip;
}

bool Host::operator!=(const Host &other) const {
  return this->host_ip != other.host_ip;
}

// boost::asio::ip::address Host::get_address() const { return host_ip; }

// Int16 Host::get_port() const { return port; }

bool Host::is_retarded() {
  std::unique_lock<std::mutex> lk{mutex};

  return ban_time_point > std::chrono::system_clock::now();
}

void Host::increase_timeout_counter() {
  std::unique_lock<std::mutex> lk{mutex};
  std::cout << "Incr";
  timeout_counter++;

  if (timeout_counter >= TIMEOUT_COUNTER_LIMIT) {
    timeout_counter = 0;
    using namespace std::literals;
    ban_time_point = std::chrono::system_clock::now() + BAN_TIME;
  }
}

std::chrono::system_clock::time_point Host::get_ban_time_point() const {
  return ban_time_point;
}

boost::asio::ip::tcp::endpoint Host::get_endpoint() const {
  return boost::asio::ip::tcp::endpoint{host_ip, port};
}

} // namespace simpleP2P