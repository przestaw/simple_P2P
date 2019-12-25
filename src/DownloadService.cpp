#include "DownloadService.h"

namespace simpleP2P::download
{

DownloadService::DownloadService(std::shared_ptr<Resource> resource) : resource(resource){
                                                                           complete_resource = std::make_shared<CompleteResource>()}

                                                                       std::thread DownloadService::init()
{
    return std::thread([=] {
        for (auto host : resource->get_hosts())
        {
            if (host->is_alive())
            {
                workers.push_back(DownloadWorker(host));
            }
        }

        for (auto &worker : workers)
        {
            worker_threads.push_back(worker.init());
        }

        for (auto &worker_thread : worker_threads)
        {
            worker_thread.join();
        }
    });
}

void DownloadService::timeout_controller()
{
    std::vector<SegmentId> previous_segment_id{workers.size(), static_cast<SegmentId>(-1)};
    for (;;)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        for (auto &worker : workers)
        {
            Host *host = worker.get_host();
            if (!host->is_retarded())
            {
                worker.wake_up();
            }

            if (worker.get_timeouted())
            {
                host->increment_timeout_counter();
            }
            else
            {
                worker.set_timeouted(true);
            }
        }
    }
}

Segment DownloadService::getSegment()
{
    complete_resource_mutex.lock();

    if (complete_resource->isCompleted())
    {
        complete_resource_mutex.unlock();
        return Segment{-1};
    }

    auto segments = complete_resource->segments();
    do
    {
        last_busy_segment = (last_busy_segment + 1) % segments;
    } while (!complete_resource->can_be_downloaded(last_busy_segment));

    Segment segment{last_busy_segment};
    complete_resource->set_busy(last_busy_segment);
    complete_resource_mutex.unlock();

    return segment;
}

void DownloadService::setSegment(SegmentId id, Uint8 data)
{
    complete_resource_mutex.lock();

    complete_resource->unset_busy(id);
    complete_resource->set_completed(id);

    complete_resource_mutex.unlock();
}

} // namespace simpleP2P::download