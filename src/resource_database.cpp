//
// Created by przemek on 09.12.2019.
//

#include "resource_database.h"
#include <utility>
#include <memory>

using std::shared_ptr;
using std::weak_ptr;

namespace simpleP2P {
    Resource_Database::Resource_Database(Host localhost) : my_host(std::move(localhost)) {}

    bool Resource_Database::has_file(const Resource &res) {
        std::shared_lock lock(database_mutex);
        auto host = std::find_if(hosts.begin(),
                                 hosts.end(),
                                 [this](shared_ptr<Host> &it) {
                                     return *(it.get()) == my_host;
                                 });
        auto res_i = std::find_if(host->get()->possesed_resources.begin(),
                                  host->get()->possesed_resources.end(),
                                  [res](weak_ptr<Resource> &it) {
                                      return *(it.lock().get()) == res;
                                  });
        return res_i != host->get()->possesed_resources.end();
    }

    void Resource_Database::add_file(const Resource &res, const Host &host) {
        std::unique_lock lock(database_mutex);
        auto res_i = std::find_if(resources.begin(),
                                  resources.end(),
                                  [res](shared_ptr<Resource> &it) {
                                      return *(it.get()) == res;
                                  });
        auto host_i = std::find_if(hosts.begin(),
                                   hosts.end(),
                                   [host](shared_ptr<Host> &it) {
                                       return *(it.get()) == host;
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

    shared_ptr<Resource> Resource_Database::who_has_file(std::vector<Int8> resource_header) {
        return who_has_file(Resource(std::move(resource_header)));
    }

    shared_ptr<Resource> Resource_Database::who_has_file(const Resource &resource_a) {
        std::shared_lock lock(database_mutex);

        auto resource = std::find_if(resources.begin(),
                                     resources.end(),
                                     [resource_a](shared_ptr<Resource> &it) {
                                         return *(it.get()) == resource_a;
                                     });
        if (resource != resources.end()) {
            return *resource;
        }
        return std::shared_ptr<Resource>(nullptr);
    }

    inline void Resource_Database::add_file(const Resource &res) {
        return add_file(res, this->my_host);
    }

    std::vector<Int8> Resource_Database::generate_database_header() {
        std::vector<Int8> header;
        header.resize(sizeof(Uint64) + 1);
        std::shared_lock lock(database_mutex);
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

    void Resource_Database::update_host(const Host &host_a) {
        std::unique_lock lock(database_mutex);
        auto host = std::find_if(hosts.begin(),
                                 hosts.end(),
                                 [&host_a](shared_ptr<Host> &it) {
                                     return *(it.get()) == host_a;
                                 });
        shared_ptr<Host> to_update = *host;
        for (auto &resource: host_a.possesed_resources) {
            if (std::count_if(
                    to_update.get()->possesed_resources.begin(),
                    to_update.get()->possesed_resources.end(),
                    [&resource](auto &it) {
                        return *(resource.lock().get()) == *(it.lock().get());
                    }) == 0) {
                add_file(*(resource.lock().get()), *(to_update.get()));
            }//else no need for action
        }
    }

    void Resource_Database::revoke_resource(const Resource &resource_a) {
        std::unique_lock lock(database_mutex);
        auto res = std::find_if(resources.begin(),
                                resources.end(),
                                [resource_a](shared_ptr<Resource> &it) {
                                    return *(it.get()) == resource_a;
                                });

        if (res != resources.end()) {
            res->get()->invalidated = true;
            std::for_each(res->get()->hosts_in_possession.begin(),
                          res->get()->hosts_in_possession.end(),
                          [&res](weak_ptr<Host> &it) {
                              it.lock().get()->remove_resource(*res);
                          });
        }
    }
}