#ifndef SIMPLE_P2P_DOWNLOADSERVICE_H
#define SIMPLE_P2P_DOWNLOADSERVICE_H

#include "DownloadWorker.h"
#include <string>
namespace simpleP2P
{

class DownloadService
{

    //DownloadWorker downloadWorkers[];

public:
    DownloadService();
    ~DownloadService();
    void download(std::string resource);
};

} // namespace simpleP2P
#endif // SIMPLE_P2P_DOWNLOADSERVICE_H
