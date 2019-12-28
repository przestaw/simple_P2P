#include "CompleteResource.h"

namespace simpleP2P::download {
Segment::Segment(){};
Segment::Segment(SegmentId id){};
Segment::~Segment(){};
SegmentId Segment::get_id() const {};
Uint8 const *Segment::get_data_ptr() const {};
} // namespace simpleP2P::download