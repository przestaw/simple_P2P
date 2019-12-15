//
// Created by przemek on 09.12.2019.
//

#include "host.h"
#include <utility>
#include "resource.h"

namespace simpleP2P {
    Host::Host(boost::asio::ip::address ip)
            : host_ip(std::move(ip)) {}

    Host::Host(std::string resource_header) {
        //TODO
    }

    std::string Host::generate_host_header() {
        return std::__cxx11::string();
    }

    bool Host::has_resource(Resource res) {
        return std::count_if(possesed_resources.begin(),
                             possesed_resources.end(),
                             [&res](Resource *it) {
                                 return res == *it;
                             }) != 0;
    }

    bool Host::operator==(const Host &other) const {
        return this->host_ip == other.host_ip;
    }

    bool Host::operator!=(const Host &other) const {
        return this->host_ip != other.host_ip;
    }
}