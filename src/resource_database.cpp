//
// Created by przemek on 09.12.2019.
//

#include "resource_database.h"
#include <utility>
#include <memory>

using std::shared_ptr;

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
            resources.emplace_back(std::make_shared<Resource>(res));
            res_i = resources.end() - 1;
        }
        if (host_i == hosts.end()) {
            hosts.emplace_back(std::make_shared<Host>(host));
            host_i = hosts.end() - 1;
        }

        //push back pointers
        res_i->get()->hosts_in_possession.emplace_back(*host_i);
        host_i->get()->possesed_resources.push_back(*res_i);
    }

    bool Resource_Database::remove_file(const Resource &res, const Host &host) {
        std::unique_lock lock(database_mutex);
        auto res_i = std::find_if(resources.begin(),
                                  resources.end(),
                                  [&res](Resource &it) {
                                      return it == res;
                                  });
        if (res_i != resources.end()) {
            auto host_i = std::find_if(res_i->get()->hosts_in_possession.begin(),
                                       res_i->get()->hosts_in_possession.end(),
                                       [&host](Host *it) {
                                           return *it == host;
                                       });
            if (host_i != res_i->get()->hosts_in_possession.end()) {
                //res_i->get()->hosts_in_possession.erase(host_i);
                //TODO: remove Host from Res and Res from Host
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    shared_ptr<Resource> Resource_Database::who_has_file(std::vector<Int8> resource_header) {
        return who_has_file(Resource(std::move(resource_header)));
    }

    shared_ptr<Resource> Resource_Database::who_has_file(const Resource &res) {
        std::shared_lock lock(database_mutex);

        auto resource = std::find(resources.begin(),
                                  resources.end(), res);
        if (resource != resources.end()) {
            return *resource;
        }
        return std::shared_ptr<Resource>(nullptr);
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
        auto host = std::find_if(hosts.begin(),
                                 hosts.end(),
                                 [this](shared_ptr<Host> &it) {
                                     return *(it.get()) == my_host;
                                 });
        header[0] = (FILE_LIST);// +1
        Uint64 size_net = host->get()->possesed_resources.size();
        //TODO htonl
        memcpy(header.data() + 1, &size_net, sizeof(size_net));
        for (auto &it : host->get()->possesed_resources) {
            auto temp = it.lock().get()->generate_resource_header();
            header.insert(header.end(), temp.begin(), temp.end());
        }
        return header;
    }
}