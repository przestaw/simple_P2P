//
// Created by przemek on 09.12.2019.
//

#ifndef SIMPLE_P2P_HOST_H
#define SIMPLE_P2P_HOST_H

#include <vector>
#include <string>
#include <boost/asio.hpp>

namespace simpleP2P {

    class Resource; //!< Forward declaration
    class UDP_Server;

    /**
     * Class contains node information and points to files it possess
     */
    class Host {
    public:
        /**
         * Constructor
         * @param ip Ip of the Host
         */
        Host(boost::asio::ip::address ip);

        /**
         * Determines if host has resource
         * @param res Resource to be checked
         * @return true if Host has Resource res
         */
        bool has_resource(Resource res);

        /**
         * Operator == checks host_ip for equality
         * @param other other
         * @return true if equal
         */
        bool operator==(const Host &other) const;

        /**
         * Operator != checks host_ip for equality
         * @param other other
         * @return true if not equal
         */
        bool operator!=(const Host &other) const;
    private:
        void remove_resource(std::shared_ptr<Resource> res);

        boost::asio::ip::address host_ip;           //!< Ip of the Host

        /*atrribs not checked for equality*/
        //TODO: timeout etc stats
        std::vector<std::weak_ptr<Resource>> possesed_resources; //!< Resources possessed by the Host

        friend class Resource_Database;             //!< friendship to manage Host's Resources timeouts etc
        friend class Udp_Server;
    };
}

#endif //SIMPLE_P2P_HOST_H
