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
        bool has_file(const Resource &res);

        /* NOTE: chosen conception
         * >>> use pointers outside Database -> slightly inefficient,
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
         * Updates the list of resources aviable from host
         * Triggered after receive of full Beacon Packet
         * @param host Host and possesed resources in a struct
         */
        void update_host(const Host &host);

        /**
         * Revokes resource and disconnects it from Hosts in database and database itself
         * Resource will still point to Hosts that possess it
         * @param resource Resource to be revoked
         */
        void revoke_resource(const Resource &resource);

        /**
         * same as add_file(Resource, Host) but host is localhost
         * @param res Resource to be added
         */
        inline void add_file(const Resource &res);

        /**
        * Returns shared pointer to resource to allow access to information about file owners
        * @param res Resource about which information is gathered
        * @return shared pointer to res
        */
        inline std::shared_ptr<Resource>
        who_has_file(std::vector<Int8> resource_header); //NOTE: no constructor available

        /**
         * Returns shared pointer to resource to allow access to information about file owners
         * @param res Resource about which information is gathered
         * @return shared pointer to res
         */
        std::shared_ptr<Resource> who_has_file(const Resource &res);

        /**
         * Generates listing of localhost content in a header
         * @return listing header of localhost resources
         */
        //TODO : max_lenght and support for partial sending
        std::vector<Int8> generate_database_header();
    private:
        Host my_host;                     //!< localhost Host struct
        /* all internal operation on this vectors must be made with pointers */
        std::vector<std::shared_ptr<Resource>> resources;  //!< vector of Resources in database
        std::vector<std::shared_ptr<Host>> hosts;          //!< vector of Hosts in database
        std::shared_mutex database_mutex; //!< rw_lock for database, allows multiple concurrent reads but permits concurrent writes
    };
}

#endif //SIMPLE_P2P_RESOURCE_DATABASE_H
