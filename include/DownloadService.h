#ifndef __DOWNLOADSERVICE_H__
#define __DOWNLOADSERVICE_H__

#include "DownloadWorker.h"
#include <string>

class DownloadService
{

    DownloadWorker downloadWorkers[];

public:
    DownloadService();
    ~DownloadService();
    void download(std::string resource);
};

#endif // __DOWNLOADSERVICE_H__
