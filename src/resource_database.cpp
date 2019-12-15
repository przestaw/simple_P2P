//
// Created by przemek on 09.12.2019.
//

#include "resource_database.h"

#include <utility>

namespace simpleP2P {
    Resource_Database::Resource_Database(Host localhost) : my_host(std::move(localhost)) {}

    bool Resource_Database::has_file(Resource res) {
        std::unique_lock lock(database_mutex);
        return false;
    }

    void Resource_Database::add_file(Resource res, Host host) {
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
        if (res_i != resources.end()) {
            resources.emplace_back(res);
            res_i = resources.end() - 1;
        }
        if (host_i != hosts.end()) {
            hosts.emplace_back(host);
            host_i = hosts.end() - 1;
        }

        //push back pointers
        res_i->host_in_possetion.push_back(&(*host_i));
        host_i->possesed_resources.push_back(&(*res_i));
    }

    bool Resource_Database::remove_file(Resource res, Host host) {
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

    std::vector<Host *> Resource_Database::who_has_file(std::string resource_header) {
        return who_has_file(Resource(std::move(resource_header)));
    }

    std::vector<Host *> Resource_Database::who_has_file(Resource res) {
        std::shared_lock lock(database_mutex);

        auto resource = std::find(resources.begin(),
                                  resources.end(), res);
        if (resource != resources.end()) {
            return std::vector(resource->host_in_possetion);
        }
        return std::vector<Host *>();
    }

    inline void Resource_Database::add_file(Resource res) {
        return add_file(std::move(res), this->my_host);
    }

    inline bool Resource_Database::remove_file(Resource res) {
        return remove_file(std::move(res), this->my_host);
    }
}