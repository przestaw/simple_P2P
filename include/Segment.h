#ifndef SIMPLE_P2P_SEGMENT_H
#define SIMPLE_P2P_SEGMENT_H

#include "GeneralTypes.h"

namespace simpleP2P::download
{
/**
 * @class Complete resource segment representation.
 * 
 */
class Segment
{
public:
    Segment();
    Segment(SegmentId id);
    ~Segment();
    SegmentId get_id() const;
    Uint8 const *get_data_ptr() const;
    static const SegmentId NO_SEGMENT = static_cast<SegmentId>(-1);

private:
    SegmentId id;
    Uint8 *data;
};

} // namespace simpleP2P::download

#endif // SIMPLE_P2P_SEGMENT_H