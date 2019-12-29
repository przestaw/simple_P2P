#ifndef SIMPLE_DOWNLOADWORKER_H
#define SIMPLE_DOWNLOADWORKER_H

#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>

#include <boost/asio.hpp>

#include "CompleteResource.h"
#include "GeneralTypes.h"
#include "Segment.h"
#include "host.h"
#include "logging_module.h"
#include "resource.h"

namespace simpleP2P::download {
class DownloadWorker {
public:
  DownloadWorker(Logging_Module &logging_module,
                 boost::asio::io_service &io_service, Host *host,
                 std::shared_ptr<CompleteResource> complete_resource);
  ~DownloadWorker();
  std::thread init();
  void check_timeout();
  void close();
  bool is_dead();

private:
  void worker();
  void connect();
  void download(Segment &segment);
  void request_segment(Segment &segment);
  void receive_segment(Segment &segment);
  Int8 *patch_segment_request(Segment &segment);
  boost::asio::io_service &io_service;
  Logging_Module &logging_module;
  boost::asio::ip::tcp::socket socket;
  std::shared_ptr<Host> host;
  std::shared_ptr<CompleteResource> complete_resource;
  std::shared_ptr<Resource> resource;
  // Host *host;
  // Resource* resource;
  // boost::asio::io_service *io_service;
  // std::shared_ptr<boost::asio::io_service> io_service;
  std::atomic<bool> dead;
  std::atomic<bool> closed;
  std::mutex cv_m;
  std::condition_variable cv;
  std::atomic<bool> timeouted;
  std::mutex timeouted_mutex;
};
} // namespace simpleP2P::download
#endif // SIMPLE_DOWNLOADWORKER_H
