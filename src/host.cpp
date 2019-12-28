//
// Created by przemek on 09.12.2019.
//

#include "host.h"
#include "resource.h"
#include <utility>

namespace simpleP2P {
Host::Host(boost::asio::ip::address ip) : host_ip(std::move(ip)) {}

Host::Host(std::string resource_header) {
  // TODO
}

std::string Host::generate_host_header() { return std::__cxx11::string(); }

    bool Host::has_resource(Resource res) {
        return std::count_if(possesed_resources.begin(),
                             possesed_resources.end(),
                             [&res](Resource *it) {
                                 return res == *it;
                             }) != 0;
    }

boost::asio::ip::address Host::get_address() const {}

Int16 Host::get_port() const { return port; }

bool Host::is_retarded() const {
  // TODO check sync
  return get_ban_time_point() > std::chrono::system_clock::now();
}

void Host::increase_timeout_counter() {
  timeout_counter++;
  // TODO:
  // ban host after timeout_counter exceeds given value
}

std::chrono::system_clock::time_point Host::get_ban_time_point() const {
  // TODO: casting ban_time_point
  // return ban_time_point;
  return std::chrono::system_clock::now();
}

} // namespace simpleP2P