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

namespace simpleP2P {
    /**
     * Class holding information about files in network and on localhost
     */
    class Resource_Database {
    public:
        /**
         * Constructor
         * @param localhost localhost
         */
        Resource_Database(Host localhost);

        /**
         * Check if localhost has certain file
         * @param res Resource to be checked
         * @return true if host already has some resource
         */
        //TODO
        bool has_file(Resource res);

        /* NOTE: chosen conception
         * >>> use pointers outside Database -> slightly inefficient, can't track life as it's not dynamically allocated
         *      probably best as it is possible to have host without resources and theoretically lonely resource
         * This assumes that once host is in database it's never deleted
         */

        /**
         * Adds connection between file and resource, adn creates them if they do not exist
         * @param res Resource to be added
         * @param host Host which possess Resource res
         */
        void add_file(const Resource &res,
                      const Host &host);

        /**
         * Removes connection between file and resource
         * @param res Resource to be removed from host list
         * @param host Host which resource will be removed
         * @return returns false if file did not existed or was not possesed
         */
        bool remove_file(const Resource &res,
                         const Host &host);

        /**
         * same as add_file(Resource, Host) but host is localhost
         * @param res Resource to be added
         */
        inline void add_file(const Resource &res);

        /**
         * same as remove_file(Resource, Host) but host is localhost
         * @param res Resource to be removed from localhost list
         * @return returns false if file did not existed or was not possesed
         */
        inline bool remove_file(const Resource &res);

        // TODO: consider returning shared_ptr for Resource
        std::vector<Host *> who_has_file(std::vector<Int8> resource_header);
        std::vector<Host *> who_has_file(const Resource &res);

        /**
         * Generates listing of localhost content in a header
         * @return listing header of localhost resources
         */
        //TODO : max_lenght and support for partial sending
        std::vector<Int8> generate_database_header();

        /**
         * Returns a vector of pointers to resources for the CLI
         * @return a vector of pointers to resources
         */
        const std::vector<Resource *>& getResources() const;
    private:
        Host my_host;                     //!< localhost Host struct
        /* all internal operation on this vectors must be made with pointers */
        std::vector<Resource> resources;  //!< vector of Resources in database
        std::vector<Host> hosts;          //!< vector of Hosts in database
        std::shared_mutex database_mutex; //!< rw_lock for database, allows multiple concurrent reads but permits concurrent writes
    };
}

#endif //SIMPLE_P2P_RESOURCE_DATABASE_H
