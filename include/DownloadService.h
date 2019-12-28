#ifndef SIMPLE_P2P_DOWNLOADSERVICE_H
#define SIMPLE_P2P_DOWNLOADSERVICE_H

#include "CompleteResource.h"
#include "DownloadWorker.h"
#include "GeneralTypes.h"
#include "resource.h"
// #include "Segment.h"
#include <memory>
#include <string>

namespace simpleP2P::download {

class DownloadService {
public:
  DownloadService(std::shared_ptr<Resource> resource);
  ~DownloadService();
  std::thread init();
  // Segment get_segment();
  // void set_segment(Segment &segment);

private:
  void timeout_controller();
  std::shared_ptr<Resource> resource;
  std::shared_ptr<CompleteResource> complete_resource;
  std::vector<std::shared_ptr<DownloadWorker>> workers;
  std::vector<std::thread> worker_threads;
  // std::mutex complete_resource_mutex;
  // SegmentId last_busy_segment;
};

} // namespace simpleP2P::download
#endif // SIMPLE_P2P_DOWNLOADSERVICE_H
