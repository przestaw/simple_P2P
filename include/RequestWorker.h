/*
 * RequestWorker.h
 * Kamil Zacharczuk
 */
#ifndef SIMPLE_P2P_REQUEST_WORKER_H
#define SIMPLE_P2P_REQUEST_WORKER_H

#include <memory>    // enable_shared_from_this
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace simpleP2P {
    /**
     * \brief TCP connection handler, created by the TCP server.
     *
     * Receives the file request, buffers requested segments and sends them to the client.
     */
    class RequestWorker
            : public std::enable_shared_from_this<RequestWorker>    // Necessary, because copies of the worker will be made by
        // the software as required.
    {
    public:
        /**
         * Constructor allows setting the socket on which the connection is established
         */
        RequestWorker(boost::asio::io_service &io_service);

        /**
         * Start handling the request.
         */
        void start();

        /**
         * Get socket.
         */
        tcp::socket &socket();

    private:
        tcp::socket _socket;    //!< Socket on which the connection is established.

        // TODO: Actual work of the worker.
    };
}

#endif // SIMPLE_P2P_REQUEST_WORKER_H
