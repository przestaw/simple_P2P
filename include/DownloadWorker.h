#ifndef SIMPLE_DOWNLOADWORKER_H
#define SIMPLE_DOWNLOADWORKER_H

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

#include <boost/asio.hpp>

#include "GeneralTypes.h"
#include "DownloadService.h"
#include "host.h"
#include "resource.h"
#include "Segment.h"
namespace simpleP2P::download
{
class DownloadWorker
{

public:
    DownloadWorker();
    ~DownloadWorker();
    std::thread init();

private:
    void worker();
    void download(Segment &segment);
    void connect();
    Host *host;
    DownloadService *downloadService;
    std::atomic<bool> alive;
    std::atomic<bool> active;
    boost::asio::io_service *io_service;
    boost::asio::ip::tcp::socket socket;
    friend DownloadService;
};
} // namespace simpleP2P::download
#endif // SIMPLE_DOWNLOADWORKER_H
