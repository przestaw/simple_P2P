#include "DownloadService.h"

namespace simpleP2P::download {

DownloadService::DownloadService(std::shared_ptr<Resource> resource)
    : resource(resource) {
  complete_resource = std::make_shared<CompleteResource>(resource);
}

std::thread DownloadService::init() {
  return std::thread([=] {
    for (auto host : resource->get_hosts()) {
      if (!host->is_retarded()) {
        workers.push_back(
            std::make_shared<DownloadWorker>(host, complete_resource));
      }
    }

    for (auto &worker : workers) {
      worker_threads.push_back(worker->init());
    }

    for (auto &worker_thread : worker_threads) {
      worker_thread.join();
    }

    std::thread controller{[=]() { timeout_controller(); }};
    controller.join();
  });
}

void DownloadService::timeout_controller() {
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    if (complete_resource->is_completed()) {
      break;
    }

    for (auto &worker : workers) {
      worker->check_timeout();
    }
  }
}

// Segment DownloadService::get_segment() {
//   std::unique_lock<std::mutex> lk{complete_resource_mutex};

//   if (complete_resource->is_completed()) {
//     return Segment::NO_SEGMENT;
//   }

//   auto segments = complete_resource->segments();

//   do {
//     last_busy_segment = (last_busy_segment + 1) % segments;
//   } while (!complete_resource->can_be_downloaded(last_busy_segment));

//   Segment segment{last_busy_segment};
//   complete_resource->set_busy(last_busy_segment);

//   return segment;
// }

// void DownloadService::set_segment(Segment &segment) {
//   std::unique_lock<std::mutex> lk{complete_resource_mutex};
//   complete_resource->unset_busy(segment.get_id());
//   complete_resource->set_completed(segment.get_id());
// }

} // namespace simpleP2P::download