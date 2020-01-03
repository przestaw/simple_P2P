#include "CompleteResource.h"

namespace simpleP2P::download {

CompleteResource::CompleteResource(std::shared_ptr<Resource> resource_c)
    : resource(resource_c),
      busy_segments(resource->calc_segments_count()),
      completed_segments(resource->calc_segments_count()), completed_counter(0),
      last_busy_segment(Segment::NO_SEGMENT_ID) {
  data = new Uint8[resource->calc_segments_count() * SEGMENT_SIZE];
}

CompleteResource::~CompleteResource() { delete[] data; }

std::shared_ptr<Resource> CompleteResource::get_resource() const {
  return resource;
}

Segment CompleteResource::get_segment() {
  std::unique_lock<std::mutex> lk{complete_resource_mutex};

  if (is_completed()) {
    return Segment{Segment::NO_SEGMENT_ID, nullptr};
  }

  auto segments = resource->calc_segments_count();

  do {
    last_busy_segment = (last_busy_segment + 1) % segments;
  } while (!can_be_downloaded(last_busy_segment));

  set_busy(last_busy_segment);

  return Segment{last_busy_segment,
                 data + last_busy_segment * SEGMENT_SIZE * sizeof(Uint8)};
}

void CompleteResource::set_segment(Segment &segment) {
  std::unique_lock<std::mutex> lk{complete_resource_mutex};
  unset_busy(segment.get_id());
  set_completed(segment.get_id());
}
bool CompleteResource::is_completed() {
  return completed_counter == resource->calc_segments_count();
}

bool CompleteResource::can_be_downloaded(SegmentId id) {
  return busy_segments[id] == false && completed_segments[id] == false;
}

void CompleteResource::set_completed(SegmentId id) {
  completed_segments[id] = true;
  completed_counter++;
}
void CompleteResource::set_busy(SegmentId id) { busy_segments[id] = true; }
void CompleteResource::unset_busy(SegmentId id) { busy_segments[id] = false; }

} // namespace simpleP2P::download