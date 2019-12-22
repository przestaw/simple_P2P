//
// Created by przemek on 09.12.2019.
//

#ifndef SIMPLE_P2P_HOST_H
#define SIMPLE_P2P_HOST_H

#include <vector>
#include <string>
#include <boost/asio.hpp>

namespace simpleP2P {

    class Resource;

    class Host { //!< contains node information and points to files it possess
    public:
        Host(boost::asio::ip::address ip);

        bool has_resource(Resource res);

        bool operator==(const Host &other) const;

        bool operator!=(const Host &other) const;
    private:
        boost::asio::ip::address host_ip;

        /*atrribs not checked for equality*/
        //TODO: timeout etc stats
        std::vector<Resource *> possesed_resources;

        friend class Resource_Database;
    };
}

#endif //SIMPLE_P2P_HOST_H
