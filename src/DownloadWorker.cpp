#include "DownloadWorker.h"

namespace simpleP2P::download {

DownloadWorker::DownloadWorker(
    std::shared_ptr<Host> host,
    std::shared_ptr<CompleteResource> complete_resource)
    : host(host), complete_resource(complete_resource), timeouted(false),
      closed(false){};

DownloadWorker::DownloadWorker(
    Host *host, std::shared_ptr<CompleteResource> complete_resource)
    : host(host), complete_resource(complete_resource), timeouted(false),
      closed(false){};

DownloadWorker::~DownloadWorker() {}

std::thread DownloadWorker::init() {
  return std::thread([=] { worker(); });
}

void DownloadWorker::worker() {
  while (true) {
    if (host->is_retarded()) {
      std::unique_lock<std::mutex> lk{cv_m};
      cv.wait_until(lk, host->get_ban_time_point(),
                    [this] { return closed == true; });
    }

    timeouted = false;

    Segment segment = complete_resource->get_segment();

    if (segment.get_id() == Segment::NO_SEGMENT) {
      break;
    }

    // download(segment);

    // TODO: handle error, unset dirty

    complete_resource->set_segment(segment);
  }
}

void DownloadWorker::close() {
  std::unique_lock<std::mutex> lk(cv_m);
  closed = true;
  cv.notify_one();
}

void DownloadWorker::connect() {
  // socket = boost::asio::ip::tcp::socket{*io_service};

  // socket.connect(
  //     boost::asio::ip::tcp::endpoint(host->get_address(), host->get_port()));
}

void DownloadWorker::request_segment(Segment &segment) {
  // boost::system::error_code error;
  // boost::asio::write(socket, boost::asio::buffer("test"), error);
}

void DownloadWorker::check_timeout() {
  std::unique_lock<std::mutex> lk{timeouted_mutex};
  if (timeouted) {
    host->increase_timeout_counter();
    timeouted = false;
  }
}

} // namespace simpleP2P::download