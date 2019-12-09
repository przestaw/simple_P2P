//
// Created by przemek on 09.12.2019.
//

#ifndef SIMPLE_P2P_RESOURCE_DATABASE_H
#define SIMPLE_P2P_RESOURCE_DATABASE_H

#include <vector>
#include <string>
#include "host.h"
#include "resource.h"

namespace simpleP2P {

    class Resource_Database {
    public:
        Resource_Database(Host host);

        bool has_file(std::string resource_header);

        bool has_file(Resource res);

        /* TODO
         * > consider std::string instead of Resource and Host
         * > pass Resource and Host as references? -> might be tricky as we cant return vector of Hosts/Requests refs
         * >>> use pointers outside Database -> slightly inefficient, can't track life as it's not dynamically allocated
         *      probably best as it is possible to have host without resources and theoretically lonely resource
         * > use dynamic memory and shared_ptr -> ?
         * OR
         * leave as-is and prepare proper operators == < > etc
         * + add
         * void fill_resource(Resource &res)
         * void fill_host(Host &host)
         */
        bool add_file(Resource res, Host host);

        bool remove_file(Resource res, Host host); //!< returns false if file did not existed

        inline bool add_file(Resource res); //!< same as add_file(Resource, Host) but host is localhost
        inline bool remove_file(Resource res); //!< same as remove_file(Resource, Host) but host is localhost

        std::vector<Host> who_has_file(std::string resource_header);

        std::vector<Host> who_has_file(Resource res);

    private:
        Host my_host;
        /* all internal operation on this vectors must be made with references */
        std::vector<Resource> resources;
        std::vector<Host> hosts;
    };
}

#endif //SIMPLE_P2P_RESOURCE_DATABASE_H
