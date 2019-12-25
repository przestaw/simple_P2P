#ifndef SIMPLE_P2P_SEGMENT_H
#define SIMPLE_P2P_SEGMENT_H

#include "GeneralTypes.h"

namespace simpleP2P::download
{

class Segment
{
public:
    Segment();
    Segment(SegmentId id);
    ~Segment();
    SegmentId getId();
    Uint8 *getDataPtr();

private:
    SegmentId id;
    Uint8 *data;
};

} // namespace simpleP2P::download

#endif // SIMPLE_P2P_SEGMENT_H
