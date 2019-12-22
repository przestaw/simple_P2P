//
// Created by przemek on 09.12.2019.
//

#include <algorithm>
#include "host.h"
#include "resource.h"
#include <cstring>

namespace simpleP2P {
    Resource::Resource(std::string name_c, Uint64 size_c, std::string path_c)
            : size(size_c), name(name_c), path(path_c) {}

    Resource::Resource(std::vector<Int8> resource_header) {

    }

    std::vector<Int8> Resource::generate_resource_header() {
        std::vector<Int8> header;
        header.resize(256 + 64);
        memset(header.data(), 0, 256);
        strcpy((char *) header.data(), getName().c_str());
        Uint64 size_net = getSize(); //TODO htonl
        memcpy(header.data(), &size_net, sizeof(size_net));
        return header;
    }

    bool Resource::has_host(simpleP2P::Host host) {
        return std::count_if(host_in_possetion.begin(),
                             host_in_possetion.end(),
                             [&host](auto it) {
                                 return *it == host;
                             }) != 0;
    }

    Uint64 Resource::getSize() const {
        return size;
    }

    const std::string &Resource::getName() const {
        return name;
    }

    const std::string &Resource::getPath() const {
        return path;
    }

    bool Resource::operator==(const Resource &other) const {
        return this->size == other.size &&
               this->name == other.name;
    }

    bool Resource::operator!=(const Resource &other) const {
        return !operator==(other);
    }
}
