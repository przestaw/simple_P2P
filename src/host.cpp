//
// Created by przemek on 09.12.2019.
//

#include "host.h"
#include <vector>
#include <utility>
#include "resource.h"

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
}