#ifndef SIMPLE_P2P_COMPLETERESOURCE_H
#define SIMPLE_P2P_COMPLETERESOURCE_H

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <string>

#include <boost/dynamic_bitset.hpp>

#include "GeneralTypes.h"
#include "Segment.h"
#include "resource.h"

namespace simpleP2P::download {

class CompleteResource {
public:
  CompleteResource(std::shared_ptr<Resource> resource_c);
  ~CompleteResource();
  std::shared_ptr<Resource> get_resource() const;
  Segment get_segment();
  void set_segment(Segment &segment);
  bool is_completed();
  void unset_busy(SegmentId id);

private:
  bool downloadable(SegmentId id);
  void set_completed(SegmentId id);
  void set_busy(SegmentId id);
  std::shared_ptr<Resource> resource;
  boost::dynamic_bitset<> busy_segments;
  boost::dynamic_bitset<> completed_segments;
  Uint16 completed_counter;
  Uint8 *data;
  std::mutex complete_resource_mutex;
  std::condition_variable cv;
};

} // namespace simpleP2P::download
#endif // SIMPLE_P2P_COMPLETERESOURCE_H
