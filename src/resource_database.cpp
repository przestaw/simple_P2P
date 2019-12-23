//
// Created by przemek on 09.12.2019.
//

#include "resource_database.h"

#include <utility>

namespace simpleP2P {
    Resource_Database::Resource_Database(Host localhost) : my_host(std::move(localhost)) {}

    bool Resource_Database::has_file(Resource res) {
        std::unique_lock lock(database_mutex);

        (void) res;
        //TODO

        return false;
    }

    void Resource_Database::add_file(const Resource &res, const Host &host) {
        std::unique_lock lock(database_mutex);
        auto res_i = std::find_if(resources.begin(),
                                  resources.end(),
                                  [&res](Resource &it) {
                                      return res == it;
                                  });
        auto host_i = std::find_if(hosts.begin(),
                                   hosts.end(),
                                   [&host](Host &it) {
                                       return host == it;
                                   });

        //create resource and host if they do not exist
        if (res_i == resources.end()) {
            resources.emplace_back(res);
            res_i = resources.end() - 1;
        }
        if (host_i == hosts.end()) {
            hosts.emplace_back(host);
            host_i = hosts.end() - 1;
        }

        //push back pointers
        res_i->host_in_possetion.push_back(&(*host_i));
        host_i->possesed_resources.push_back(&(*res_i));
    }

    bool Resource_Database::remove_file(const Resource &res, const Host &host) {
        std::unique_lock lock(database_mutex);
        auto res_i = std::find_if(resources.begin(),
                                  resources.end(),
                                  [&res](Resource &it) {
                                      return it == res;
                                  });
        if (res_i != resources.end()) {
            auto host_i = std::find_if(res_i->host_in_possetion.begin(),
                                       res_i->host_in_possetion.end(),
                                       [&host](Host *it) {
                                           return *it == host;
                                       });
            if (host_i != res_i->host_in_possetion.end()) {
                res_i->host_in_possetion.erase(host_i);
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    std::vector<Host *> Resource_Database::who_has_file(std::vector<Int8> resource_header) {
        return who_has_file(Resource(std::move(resource_header)));
    }

    std::vector<Host *> Resource_Database::who_has_file(const Resource &res) {
        std::shared_lock lock(database_mutex);

        auto resource = std::find(resources.begin(),
                                  resources.end(), res);
        if (resource != resources.end()) {
            return std::vector(resource->host_in_possetion);
        }
        return std::vector<Host *>();
    }

    inline void Resource_Database::add_file(const Resource &res) {
        return add_file(res, this->my_host);
    }

    inline bool Resource_Database::remove_file(const Resource &res) {
        return remove_file(res, this->my_host);
    }

    std::vector<Int8> Resource_Database::generate_database_header() {
        std::vector<Int8> header;
        header.resize(sizeof(Uint64) + 1);
        auto host = std::find(hosts.begin(),
                              hosts.end(), my_host);
        header[0] = (FILE_LIST);// +1
        Uint64 size_net = host->possesed_resources.size();
        //TODO htonl
        memcpy(header.data() + 1, &size_net, sizeof(size_net));
        for (auto &it : host->possesed_resources) {
            auto temp = it->generate_resource_header();
            header.insert(header.end(), temp.begin(), temp.end());
        }
        return header;
    }
}