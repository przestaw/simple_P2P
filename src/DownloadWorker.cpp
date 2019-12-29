#include "DownloadWorker.h"
#include <sstream>

namespace simpleP2P::download {

DownloadWorker::DownloadWorker(
    Logging_Module &logging_module, boost::asio::io_service &io_service,
    Host *host, std::shared_ptr<CompleteResource> complete_resource)
    : logging_module(logging_module), io_service(io_service), host(host),
      complete_resource(complete_resource), socket(io_service),
      timeouted(false), closed(false), dead(false){};

DownloadWorker::~DownloadWorker() {
  if (socket.is_open()) {
    socket.close();
  }
}

std::thread DownloadWorker::init() {
  return std::thread([=] {
    try {
      connect();
      worker();
    } catch (std::exception &e) {
      dead = true;
      std::stringstream error_message;
      error_message << "Download worker terminated, host:"
                    << host->get_endpoint() << std::endl
                    << " detailed error: " << e.what() << std::endl;

      using namespace std::chrono;
      logging_module.add_log_line(error_message.str(),
                                  system_clock::to_time_t(system_clock::now()));
    }
  });
}

void DownloadWorker::worker() {
  while (true) {
    if (host->is_retarded()) {
      std::unique_lock<std::mutex> lk{cv_m};
      cv.wait_until(lk, host->get_ban_time_point(),
                    [this]() { return closed == true; });
    }

    timeouted = false;

    Segment segment = complete_resource->get_segment();

    if (segment.get_id() == Segment::NO_SEGMENT) {
      break;
    }

    download(segment);

    complete_resource->set_segment(segment);
  }
}

void DownloadWorker::close() {
  std::unique_lock<std::mutex> lk(cv_m);
  closed = true;
  cv.notify_one();
}

void DownloadWorker::connect() {
  try {
    socket.connect(host->get_endpoint()); // TODO errors
  } catch (std::exception &e) {
    std::stringstream error_message;
    error_message << "Failed to connect to host" << host->get_endpoint()
                  << std::endl
                  << " detailed error: " << e.what() << std::endl;

    using namespace std::chrono;
    logging_module.add_log_line(error_message.str(),
                                system_clock::to_time_t(system_clock::now()));
    throw std::exception();
  }
}

void DownloadWorker::download(Segment &segment) {
  try {
    request_segment(segment);
    receive_segment(segment);
  } catch (std::exception &e) {
    std::stringstream error_message;
    error_message << "Failed to download segment: " + segment.get_id()
                  << std::endl
                  << " of resource: " << resource->getName() << std::endl
                  << " from host " << host->get_endpoint() << std::endl
                  << " detailed error: " << e.what() << std::endl;

    using namespace std::chrono;
    logging_module.add_log_line(error_message.str(),
                                system_clock::to_time_t(system_clock::now()));
  }
}

void DownloadWorker::request_segment(Segment &segment) {
  boost::system::error_code error;
  // size!
  boost::asio::write(socket, boost::asio::buffer("test"), error);
  if (error) {
    throw boost::system::system_error(error);
  }
}

void DownloadWorker::receive_segment(Segment &segment) {
  boost::system::error_code error;
  // boost::asio::read(socket,
  // boost::asio::buffer(patch_segment_request(segment), error);
  // TODO check for errors
  if (error) {
    throw boost::system::system_error(error);
  }
}

Int8 *patch_segment_request(Segment &segment) { return nullptr; }

void DownloadWorker::check_timeout() {
  std::unique_lock<std::mutex> lk{timeouted_mutex};
  if (timeouted) {
    host->increase_timeout_counter();
    timeouted = false;
  }
}

bool DownloadWorker::is_dead() { return dead; }

} // namespace simpleP2P::download