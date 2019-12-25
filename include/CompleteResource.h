#ifndef SIMPLE_P2P_COMPLETERESOURCE_H
#define SIMPLE_P2P_COMPLETERESOURCE_H

#include <string>
#include <boost/dynamic_bitset.hpp>
#include "DownloadWorker.h"
#include "GeneralTypes.h"

namespace simpleP2P::download
{

class CompleteResource
{
public:
    CompleteResource(Resource &resource);
    ~CompleteResource();
    bool isCompleted();
    bool can_be_downloaded(SegmentId id);
    void set_completed(SegmentId id);
    void set_busy(SegmentId id);
    void unset_busy(SegmentId id);

private:
    void worker();
    bool completed;
    Resource *resource;
    boost::dynamic_bitset<> busy_segments;
    boost::dynamic_bitset<> completed_segments;
    Uint8 *data;
};

} // namespace simpleP2P::download
#endif // SIMPLE_P2P_COMPLETERESOURCE_H
