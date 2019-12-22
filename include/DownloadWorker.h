#ifndef SIMPLE_DOWNLOADWORKER_H
#define SIMPLE_DOWNLOADWORKER_H

#include "GeneralTypes.h"
#include "Host.h"
#include "Resource.h"
namespace simpleP2P
{

class DownloadWorker
{
public:
    DownloadWorker();
    ~DownloadWorker();
    const Int8 *download(const Host &host, const Resource &resource, Int64 segment);
};
} // namespace simpleP2P
#endif // SIMPLE_DOWNLOADWORKER_H
