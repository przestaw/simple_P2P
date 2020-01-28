//
// Created by przemek on 09.12.2019.
//

#include "Host.h"
#include <vector>
#include <utility>
#include "Resource.h"
#include <iostream>
#include <utility>

namespace simpleP2P {
Host::Host(boost::asio::ip::address ip)
    : host_ip(std::move(ip)), timeout_counter(0), retarded(false), no_of_missed_updates(0) {}

bool Host::has_resource(const Resource &resource) {
  std::shared_lock lock(host_mutex);
  return std::count_if(
      possesed_resources.begin(),
      possesed_resources.end(),
      [&resource](auto &it) {
        return resource == *(it.lock().get());
      }) != 0;
}

bool Host::operator==(const Host &other) const {
  std::shared_lock lock(host_mutex);
  return this->host_ip == other.host_ip;
}

bool Host::operator!=(const Host &other) const {
  std::shared_lock lock(host_mutex);
  return this->host_ip != other.host_ip;
}

void Host::remove_resource(const Resource &res) {
  std::unique_lock lock(host_mutex);
  possesed_resources.erase(
      std::remove_if(
          possesed_resources.begin(),
          possesed_resources.end(),
          [&res](auto &it) {
            return *it.lock() == res;
          }
      ), possesed_resources.end());
}

std::vector<std::weak_ptr<Resource>> Host::get_possesed() const {
  std::shared_lock lock(host_mutex);
  return possesed_resources;
}

bool Host::is_retarded() {
  std::shared_lock lock(host_mutex);
  return retarded;
}

void Host::increase_timeout_counter() {
  std::unique_lock lock(host_mutex);
  timeout_counter++;
  if (timeout_counter >= TIMEOUT_COUNTER_LIMIT) {
    retarded = true;
    using namespace std::literals;
    ban_time = std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now() + BAN_TIME);
  }
}

std::chrono::system_clock::time_point Host::get_ban_time_point() const {
  std::shared_lock lock(host_mutex);
  return std::chrono::system_clock::from_time_t(ban_time);
}

boost::asio::ip::tcp::endpoint Host::get_endpoint() const {
  std::shared_lock lock(host_mutex);
  return boost::asio::ip::tcp::endpoint{host_ip, TCP_SERVER_PORT};
}

uint16_t Host::get_missed_updates() const {
  std::shared_lock lock(host_mutex);
  return no_of_missed_updates;
}

void Host::reset_missed_updates() {
  std::unique_lock lock(host_mutex);
  no_of_missed_updates = 0;
}

void Host::add_resource(const std::shared_ptr<Resource> &res) {
  std::unique_lock lock(host_mutex);
  possesed_resources.push_back(res);
}

void Host::inc_missed_updates() {
  std::unique_lock lock(host_mutex);
  ++no_of_missed_updates;
}
void Host::remove_all_resources() {
  std::unique_lock lock(host_mutex);
  possesed_resources.clear();
}

} // namespace simpleP2P
