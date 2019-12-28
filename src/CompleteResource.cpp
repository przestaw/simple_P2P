#include "CompleteResource.h"

namespace simpleP2P::download {

CompleteResource::CompleteResource(std::shared_ptr<Resource> resource)
    : resource(resource), completed(false) {}

CompleteResource::~CompleteResource(){};

Segment CompleteResource::get_segment() {
  std::unique_lock<std::mutex> lk{complete_resource_mutex};

  if (is_completed()) {
    return Segment::NO_SEGMENT;
  }

  auto segments = resource->calc_segments_count();

  do {
    last_busy_segment = (last_busy_segment + 1) % segments;
  } while (!can_be_downloaded(last_busy_segment));

  Segment segment{last_busy_segment};
  set_busy(last_busy_segment);

  return segment;
}

void CompleteResource::set_segment(Segment &segment) {
  std::unique_lock<std::mutex> lk{complete_resource_mutex};
  unset_busy(segment.get_id());
  set_completed(segment.get_id());
}
bool CompleteResource::is_completed(){};
bool CompleteResource::can_be_downloaded(SegmentId id){};
void CompleteResource::set_completed(SegmentId id){};
void CompleteResource::set_busy(SegmentId id){};
void CompleteResource::unset_busy(SegmentId id){};

} // namespace simpleP2P::download