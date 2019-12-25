#ifndef SIMPLE_P2P_DOWNLOADSERVICE_H
#define SIMPLE_P2P_DOWNLOADSERVICE_H

#include "DownloadWorker.h"
#include "GeneralTypes.h"
#include "CompleteResource.h"
#include "Segment.h"
#include <string>
#include <memory>

namespace simpleP2P::download
{

class DownloadService
{
public:
    DownloadService(std::shared_ptr<Resource> resource);
    ~DownloadService();
    std::thread init();
    Segment getSegment();
    void setSegment(Segment segment);

private:
    void timeout_controller();
    std::shared_ptr<Resource> resource;
    std::shared_ptr<CompleteResource> complete_resource;
    std::vector<std::thread> workers;
    std::mutex complete_resource_mutex;
    SegmentId last_busy_segment;
    std::vector<DownloadWorker> workers;
    std::vector<std::thread> worker_threads;

    friend DownloadWorker;
};

} // namespace simpleP2P::download
#endif // SIMPLE_P2P_DOWNLOADSERVICE_H
