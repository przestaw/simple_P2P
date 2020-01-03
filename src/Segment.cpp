#include "CompleteResource.h"

namespace simpleP2P::download {
// TODO INIT DATA PTR
Segment::Segment(SegmentId id_c, Uint8 *data_c) : id(id_c), data(data_c) {}
Segment::~Segment() {}
SegmentId Segment::get_id() const { return id; }
Uint8 const *Segment::get_data_ptr() const { return data; }
} // namespace simpleP2P::download