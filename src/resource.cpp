//
// Created by przemek on 09.12.2019.
//

#include <algorithm>
#include "host.h"
#include "resource.h"
#include <cstring>
#include <iostream>

namespace simpleP2P {
    Resource::Resource(std::string name_c, Uint64 size_c, std::string path_c)
            : size(size_c), name(name_c), invalidated(false), path(path_c) {}

    Resource::Resource(std::vector<Int8> resource_header) : invalidated(false) {
        (void) resource_header;
        //TODO
    }

    std::vector<Int8> Resource::generate_resource_header() {
        std::vector<Int8> header;
        header.resize(256 + sizeof(Uint64));
        memset(header.data(), 0, 256);

        memcpy(header.data(), name.c_str(), name.size());

        Uint64 size_net = htobe64(getSize());
        memcpy(header.data() + 256, &size_net, sizeof(size_net));

        return header;
    }

    bool Resource::has_host(simpleP2P::Host host) {
        return std::count_if(hosts_in_possession.begin(),
                             hosts_in_possession.end(),
                             [&host](std::weak_ptr<Host> &it) {
                                 return *(it.lock().get()) == host;
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

    void Resource::remove_host(std::shared_ptr<Host> host) {
        //make local copy and erase
        std::vector<std::weak_ptr<Host>> temp(hosts_in_possession.begin(),
                                              hosts_in_possession.end());
        temp.erase(
                std::remove_if(
                        temp.begin(),
                        temp.end(),
                        [&host](auto &it) {
                            return it.lock() == host;
                        }
                ), temp.end());

        //swap contents of the vectors
        hosts_in_possession.clear();
        hosts_in_possession.assign(temp.begin(), temp.end());
    }
}
