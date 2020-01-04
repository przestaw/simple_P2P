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
  DownloadWorker(Logging_Module &logging_module_c,
                 boost::asio::io_service &io_service_c, Host *host_c,
                 std::shared_ptr<CompleteResource> complete_resource_c);
  ~DownloadWorker();
  std::thread init();
  void check_timeout();
  void close();
  bool is_closed();

private:
  void worker();
  void connect();
  void download(Segment &segment);
  void request_segment(Segment &segment);
  void receive_segment(Segment &segment);
  Int8 *patch_segment_request(Segment &segment);
  void log_timeout();
  void log_start_downloading();
  void log_finish_downloading();
  std::string get_log_header();
  Logging_Module &logging_module;
  boost::asio::io_service &io_service;
  std::shared_ptr<Host> host;
  std::shared_ptr<CompleteResource> complete_resource;
  boost::asio::ip::tcp::socket socket;
  std::atomic<bool> timeouted;
  std::atomic<bool> closed;
  std::atomic<SegmentId> owned_segment_id;
  std::mutex cv_m;
  std::condition_variable cv;
  std::mutex timeouted_mutex;
};
} // namespace simpleP2P::download
#endif // SIMPLE_DOWNLOADWORKER_H
