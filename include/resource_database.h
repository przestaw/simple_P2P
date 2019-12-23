//
// Created by przemek on 09.12.2019.
//

#ifndef SIMPLE_P2P_RESOURCE_DATABASE_H
#define SIMPLE_P2P_RESOURCE_DATABASE_H

#include <vector>
#include <string>
#include <shared_mutex>
#include "host.h"
#include "resource.h"

#define FILE_LIST 15

namespace simpleP2P {

    class Resource_Database {
    public:
        Resource_Database(Host localhost);

        bool has_file(Resource res);

        /* NOTE: chosen conception
         * >>> use pointers outside Database -> slightly inefficient, can't track life as it's not dynamically allocated
         *      probably best as it is possible to have host without resources and theoretically lonely resource
         * This assumes that once host is in database it's never deleted
         */
        void add_file(const Resource &res, const Host &host);

        bool remove_file(const Resource &res,
                         const Host &host); //!< returns false if file did not existed or was not possesed

        inline void add_file(const Resource &res); //!< same as add_file(Resource, Host) but host is localhost
        inline bool remove_file(const Resource &res); //!< same as remove_file(Resource, Host) but host is localhost

        std::vector<Host *> who_has_file(std::string resource_header);

        std::vector<Host *> who_has_file(const Resource &res);

        std::vector<Int8> generate_res_header();
    private:
        Host my_host;
        /* all internal operation on this vectors must be made with references */
        std::vector<Resource> resources;
        std::vector<Host> hosts;
        std::shared_mutex database_mutex;
    };
}

#endif //SIMPLE_P2P_RESOURCE_DATABASE_H
