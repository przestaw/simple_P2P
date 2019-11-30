#ifndef __DOWNLOADWORKER_H__
#define __DOWNLOADWORKER_H__

#include "GeneralTypes.h"
#include "Peer.h"
#include "ResourceHeader.h"

class DownloadWorker
{
public:
    DownloadWorker();
    ~DownloadWorker();
    const Int8 *download(const Peer &peer, const ResourceHeader &resourceHeader, Int64 segment);
};

#endif // __DOWNLOADWORKER_H__
