//
// Created by przemek on 22.12.2019.
//

#include <boost/bind.hpp>
#include <iostream>
#include "udp_client.h"

using namespace boost::asio;

simpleP2P::Udp_Client::Udp_Client(io_service &io_service,
                                  const ip::address &broadcast_address, Uint16 broadcast_port,
                                  uint_fast32_t timeout)
        : endpoint_(broadcast_address, broadcast_port),
          socket_(io_service, endpoint_.protocol()),
          tx_queue_(),
          timer(io_service, boost::posix_time::seconds(timeout)) {

    socket_.set_option(ip::udp::socket::reuse_address(true));
    socket_.set_option(socket_base::broadcast(true));

    timer.async_wait(boost::bind(&Udp_Client::fire_beacon,
                                 this));
}

simpleP2P::Udp_Client::~Udp_Client() {
    socket_.close();
}

void simpleP2P::Udp_Client::send(const std::vector<Int8> &packet) {
    bool queue_empty(tx_queue_.empty());
    tx_queue_.emplace_back(packet);
    if (queue_empty)
        transmit();
}

void simpleP2P::Udp_Client::transmit() {
    socket_.async_send_to(
            boost::asio::buffer(&tx_queue_.front()[0], tx_queue_.front().size()),
            endpoint_,
            boost::bind(&Udp_Client::write_callback,
                        boost::weak_ptr<Udp_Client>(shared_from_this()),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
}

void simpleP2P::Udp_Client::write_handler(boost::system::error_code const &error,
                                          size_t bytes_transferred) {
    tx_queue_.pop_front();
    std::cerr << "write : " << bytes_transferred << " bytes\n"; // TODO: log send
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
    (void) resource;
    //TODO : gen header
    //TODO : put in queue
}

void simpleP2P::Udp_Client::fire_beacon() {
    //TODO : generate packet
    //TODO : put in queue

    std::cerr << "blah \n";

    std::vector<Int8> vec;
    std::string blah = "blah 123456789";
    vec.insert(vec.end(), blah.begin(), blah.end());
    vec.push_back(0);
    send(vec);
    boost::posix_time::seconds interval(15);
    timer.expires_at(timer.expires_at() + interval);
    timer.async_wait(boost::bind(&Udp_Client::fire_beacon,
                                 this));
}
