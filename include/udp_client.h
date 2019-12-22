//
// Created by przemek on 22.12.2019.
//

#ifndef SIMPLE_P2P_UDP_CLIENT_H
#define SIMPLE_P2P_UDP_CLIENT_H

#include <boost/asio/ip/udp.hpp>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <deque>
#include "GeneralTypes.h"
#include "resource.h"

namespace simpleP2P {
    class Udp_Client : public boost::enable_shared_from_this<Udp_Client> {
    public:
        Udp_Client(boost::asio::io_service &io_service, Uint16 port, Uint32 timeout = 5 * 60);

        ~Udp_Client();

        void revoke_file(Resource resource);

    private:
        void fire_beacon();

#if defined(BOOST_ASIO_HAS_MOVE)

        /// Put arbitrary message in a queue
        void send(const std::vector<Int8> &packet);

        void transmit();

        /// The function called whenever a write event is received.
        void write_handler(boost::system::error_code const &error,
                           size_t /* bytes_transferred */);
//        {
//            tx_queue_.pop_front();
//            if (error)
//                ; // handle the error, it may be a disconnect.
//            else
//            if (!tx_queue_.empty())
//                transmit();
//        }

        /// Static callback function.
        /// It ensures that the object still exists and the event is valid
        /// before calling the write handler.
        static void write_callback(boost::weak_ptr<Udp_Client> ptr,
                                   boost::system::error_code const &error,
                                   size_t bytes_transferred);
//        {
//            boost::shared_ptr<Udp_Client> pointer(ptr.lock());
//            if (pointer && (boost::asio::error::operation_aborted != error))
//                pointer->write_handler(error, bytes_transferred);
//        }


        boost::asio::ip::udp::socket socket_;
        std::deque<std::vector<Int8> > tx_queue_;
        boost::asio::deadline_timer timer; //timer for the beacon
    };
}

#endif //SIMPLE_P2P_UDP_CLIENT_H
