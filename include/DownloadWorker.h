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
#include "resource.h"

namespace simpleP2P::download {
class DownloadWorker {
public:
  DownloadWorker(std::shared_ptr<Host> host,
                 std::shared_ptr<CompleteResource> complete_resource);
  DownloadWorker(Host *host,
                 std::shared_ptr<CompleteResource> complete_resource);
  ~DownloadWorker();
  std::thread init();
  void check_timeout();
  void close();

private:
  void worker();
  void connect();
  void download(Segment &segment);
  void request_segment(Segment &segment);
  std::shared_ptr<Host> host;
  std::shared_ptr<CompleteResource> complete_resource;
  std::shared_ptr<Resource> resource;
  // Host *host;
  // DownloadService *downloadService;
  // Resource* resource;
  // boost::asio::io_service *io_service;
  // std::shared_ptr<boost::asio::io_service> io_service;
  // boost::asio::ip::tcp::socket socket;
  std::atomic<bool> closed;
  std::mutex cv_m;
  std::condition_variable cv;
  std::atomic<bool> timeouted;
  std::mutex timeouted_mutex;
};
} // namespace simpleP2P::download
#endif // SIMPLE_DOWNLOADWORKER_H
