//
// Created by przemek on 04.12.2019.
//

#ifndef SIMPLE_P2P_UDP_BEACON_H
#define SIMPLE_P2P_UDP_BEACON_H

#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>
#include <vector>
#include <string>
#include <boost/asio.hpp>

namespace simpleP2P {
    class UDP_Beacon {
    public:
        UDP_Beacon(boost::asio::ip::udp::socket socket_c);

        std::thread init();

        void revoke_file(
                std::string resource);         //!< to sent aproperiate revoke header TODO: construct resource struct and create command in place
    private:
        boost::asio::ip::udp::socket socket;

        void
        fire_beacon();                                      //!< puts data in queue fired by timer TODO: internal slot for firing the beacon
        void worker();                                           //!< simple working thread for sending data
        static std::queue<std::string> datagrams_queue;          //!< Queue of data to be send ASAP
        // NOTE: std::string can properly hold octet datagram
        static std::mutex queue_mutex;                           //!< Mutex to secure exclusive access to the queue
        static std::condition_variable queue_cond;               //!< to signal internally that some data is ready to be send
    };
}

#endif //SIMPLE_P2P_UDP_BEACON_H
