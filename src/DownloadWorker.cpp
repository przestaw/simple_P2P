#include "DownloadWorker.h"

namespace simpleP2P::download
{

std::thread DownloadWorker::init()
{
    return std::thread([=] { worker(); });
}

void DownloadWorker::worker()
{
    while (active)
    {
        if (alive)
        {
            std::this_thread::sleep_until(std::chrono::time_point(0));
        }

        Segment segment = downloadService->getSegment();
        if (segment.getId() == static_cast<SegmentId>(-1))
        {
            break;
        }
        download(segment);
        downloadService->setSegment(segment);
    }
}

void DownloadWorker::connect()
{
    socket = boost::asio::iptcp::socket{*io_service};

    socket.connect(boost::asio::ip::tcp::endpoint(host.get_address(), host.get_port()));
}

void DownloadWorker::request_segment(Segment &segment)
{
    boost::system::error_code error;
    boost::asio::write(socket, boost::asio::buffer(msg), error);
}

void DownloadWorker::parse_seg_req(Segment &segment)
{
    // COMMANDS.REQ_SEGMENT + downloadService->resource->generate_resource_header() + segment.getId()
}

} // namespace simpleP2P::download