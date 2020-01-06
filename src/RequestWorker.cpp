/*
 * RequestWorker.cpp
 * Kamil Zacharczuk
 */
#include "RequestWorker.h"
#include <boost/asio.hpp>
#include <utility> // move

using boost::asio::ip::tcp;

namespace simpleP2P {
    RequestWorker::RequestWorker(boost::asio::io_service &io_service)
            : _socket(io_service) {}

    void RequestWorker::start() {
        // TODO: Do the work.
    }

    tcp::socket &RequestWorker::socket() { return _socket; }

// TODO: The actual work.
} // namespace simpleP2P
