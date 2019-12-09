//
// Created by przemek on 09.12.2019.
//

#ifndef SIMPLE_P2P_HOST_H
#define SIMPLE_P2P_HOST_H

#include <vector>
#include <string>


namespace simpleP2P {

    class Resource;

    class Host { //!< contains node information and points to files it possess
    public:
        Host(std::string resource_header);

        std::string generate_host_header();

        bool add_resource(Resource res);

        bool has_resource(Resource res);

    private:
        //TODO: IP and/or MAC
        /*atrribs not checked for equality*/
        //TODO: timeout etc stats
        std::vector<Resource> possesed_resources;
    };
}

#endif //SIMPLE_P2P_HOST_H
