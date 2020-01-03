#ifndef SIMPLE_P2P_DOWNLOADSERVICE_H
#define SIMPLE_P2P_DOWNLOADSERVICE_H

#include "CompleteResource.h"
#include "DownloadWorker.h"
#include "FileManager.h"
#include "GeneralTypes.h"
#include "logging_module.h"
#include "resource.h"
#include <boost/asio.hpp>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace simpleP2P::download {

class DownloadService {
public:
  DownloadService(Logging_Module &logging_module_c,
                  boost::asio::io_service &io_service_c,
                  FileManager &file_manager_c,
                  std::shared_ptr<Resource> resource_c);
  ~DownloadService();
  std::thread init();

private:
  void create_workers();
  void init_workers();
  void controll_workers();
  void join_workers();
  void close_workers();
  void check_workers_timeout();
  bool all_workers_dead();
  void store_file();
  void handle_exception(std::exception &e);

  Logging_Module &logging_module;
  boost::asio::io_service &io_service;
  FileManager &file_manager;
  std::shared_ptr<Resource> resource;
  std::shared_ptr<CompleteResource> complete_resource;
  std::vector<std::shared_ptr<DownloadWorker>> workers;
  std::vector<std::thread> worker_threads;
  std::mutex cv_m;
  std::condition_variable cv;
};

} // namespace simpleP2P::download
#endif // SIMPLE_P2P_DOWNLOADSERVICE_H
