//
// Created by przemek on 09.12.2019.
//

#include "ResourceDatabase.h"
#include <utility>
#include <memory>
#include <iostream>

using std::shared_ptr;
using std::weak_ptr;

namespace simpleP2P {
ResourceDatabase::ResourceDatabase(Host localhost) : my_host(std::make_shared<Host>(localhost)) {
  hosts.push_back(my_host);
}

void ResourceDatabase::add_file(const Resource &res, const Host &host) {
  std::unique_lock lock(database_mutex);
  this->add_file_internal(res, host);
}

void ResourceDatabase::remove_file(const Resource &res, const Host &host) {
  std::unique_lock lock(database_mutex);
  auto res_i = std::find_if(resources.begin(),
                            resources.end(),
                            [&res](auto &it) {
                              return *(it.get()) == res;
                            });
  if (res_i != resources.end()) {
    res_i->get()->remove_host(host);
  }
}

shared_ptr<Resource> ResourceDatabase::who_has_file(std::vector<Uint8> resource_header) {
  return who_has_file(Resource(std::move(resource_header)));
}

shared_ptr<Resource> ResourceDatabase::who_has_file(const Resource &resource_a) {
  std::shared_lock lock(database_mutex);

  auto resource = std::find_if(resources.begin(),
                               resources.end(),
                               [&resource_a](shared_ptr<Resource> &it) {
                                 return *(it.get()) == resource_a;
                               });
  if (resource != resources.end()) {
    return *resource;
  }
  return std::shared_ptr<Resource>(nullptr);
}

void ResourceDatabase::add_file(const Resource &res) {
  return add_file(res, *my_host);
}

void ResourceDatabase::remove_file(const Resource &res) {
  remove_file(res, *my_host);
}

std::vector<std::vector<Uint8>> ResourceDatabase::generate_database_headers() {
  std::vector<std::vector<Uint8>> header;
  std::shared_lock lock(database_mutex);

  for (auto &it : my_host->get_possesed()) {
    header.emplace_back(it.lock()->generate_resource_header());
  }

  return header;
}

void ResourceDatabase::update_host(const Host &host_a) {
  std::unique_lock lock(database_mutex);
  auto host = std::find_if(hosts.begin(),
                           hosts.end(),
                           [&host_a](shared_ptr<Host> &it) {
                             return *(it.get()) == host_a;
                           });
  if (host == hosts.end()) {
    hosts.emplace_back(std::make_shared<Host>(host_a));
    host = hosts.end() - 1;
  }

  shared_ptr<Host> to_update = *host;
  to_update->reset_missed_updates();

  for (auto &resource: host_a.get_possesed()) {

    if (!to_update->has_resource(*(resource.lock().get()))) {
      add_file_internal(*(resource.lock().get()), *(to_update.get()));
    }//else no need for action
  }
}

void ResourceDatabase::revoke_resource(const Resource &resource_a) {
  std::unique_lock lock(database_mutex);
  auto res = std::find_if(resources.begin(),
                          resources.end(),
                          [&resource_a](shared_ptr<Resource> &it) {
                            return *(it.get()) == resource_a;
                          });

  if (res != resources.end()) {
    res->get()->set_revoked();
    auto hosts_in_possession = res->get()->get_hosts();
    std::for_each(hosts_in_possession.begin(),
                  hosts_in_possession.end(),
                  [&resource_a](std::weak_ptr<Host> &it) {
                    it.lock()->remove_resource(resource_a);
                  });
  }
}

std::shared_ptr<Host> ResourceDatabase::get_localhost() const {
  return my_host;
}

std::vector<std::shared_ptr<Resource>> ResourceDatabase::getResources() const {
  std::shared_lock lock(database_mutex);
  return resources;
}

void ResourceDatabase::check_for_gone_hosts(uint16_t left_margin) {
  std::unique_lock lock(database_mutex);
  for (auto &it : hosts) {
    it->inc_missed_updates();
    if (it->get_missed_updates() >= left_margin) {
      remove_host(it);
    }
  }
}

void ResourceDatabase::remove_host(const std::shared_ptr<Host> &host) {
  // no lock -> inline slave method
  if (host != my_host) {
    for (auto &it: host->get_possesed()) {
      it.lock()->remove_host(*host);
    }
    //NOTE: host should not be erased, just disconnected from resources
    host->remove_all_resources();
  }
}

void ResourceDatabase::add_file_internal(const Resource &res, const Host &host) {
  // no lock
  auto res_i = std::find_if(resources.begin(),
                            resources.end(),
                            [&res](shared_ptr<Resource> &it) {
                              return *(it.get()) == res;
                            });
  auto host_i = std::find_if(hosts.begin(),
                             hosts.end(),
                             [&host](shared_ptr<Host> &it) {
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
  res_i->get()->add_host(*host_i);
  host_i->get()->add_resource(*res_i);
}
}
