//
// Created by przemek on 22.12.2019.
//

#include <boost/bind.hpp>
#include "udp_client.h"

using namespace boost::asio;

simpleP2P::Udp_Client::Udp_Client(boost::asio::io_service &io_service, Uint16 port, Uint32 timeout)
        : socket_(io_service, ip::udp::endpoint(ip::udp::v4(), port)), tx_queue_(),
          timer(io_service, boost::posix_time::seconds(timeout)) {
    socket_.set_option(ip::udp::socket::reuse_address(true));
    timer.async_wait(boost::bind(&Udp_Client::fire_beacon,
                                 this));
}

simpleP2P::Udp_Client::~Udp_Client() {
    socket_.close();
}

void simpleP2P::Udp_Client::send(const std::vector<Int8> &packet) {
    bool queue_empty(tx_queue_.empty());
    tx_queue_.push_back(packet);
    if (queue_empty)
        transmit();
}

void simpleP2P::Udp_Client::transmit() {
    socket_.async_send(
            boost::asio::buffer(&tx_queue_.front()[0], tx_queue_.front().size()),
            boost::bind(&Udp_Client::write_callback,
                        boost::weak_ptr<Udp_Client>(shared_from_this()),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
}

void simpleP2P::Udp_Client::write_handler(boost::system::error_code const &error,
                                          size_t /* bytes_transferred */) {
    tx_queue_.pop_front();
    if (!error) {
        if (!tx_queue_.empty())
            transmit();
    }
}

void simpleP2P::Udp_Client::write_callback(boost::weak_ptr<Udp_Client> ptr,
                                           boost::system::error_code const &error,
                                           size_t bytes_transferred) {
    boost::shared_ptr<Udp_Client> pointer(ptr.lock());
    if (pointer && (boost::asio::error::operation_aborted != error))
        pointer->write_handler(error, bytes_transferred);
}

void simpleP2P::Udp_Client::revoke_file(simpleP2P::Resource resource) {
    //TODO : gen header
    //TODO : put in queue
}

void simpleP2P::Udp_Client::fire_beacon() {
    //TODO : generate packet
    //TODO : put in queue
    timer.async_wait(boost::bind(&Udp_Client::fire_beacon,
                                 this));
}
