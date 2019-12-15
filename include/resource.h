//
// Created by przemek on 09.12.2019.
//

#ifndef SIMPLE_P2P_RESOURCE_H
#define SIMPLE_P2P_RESOURCE_H

#include <vector>
#include <string>
#include <GeneralTypes.h>

#define SEGMENT_SIZE 1024 //1kb

namespace simpleP2P {
    class Host;

    class Resource { //!< contains file information and points to nodes with file possesion
    public:
        Resource(std::string name, Uint64 size, std::string path = "/");
        Resource(std::string resource_header);

        std::string generate_resource_header();
        bool has_host(Host host);

        Uint16 segments() {
            if (size % SEGMENT_SIZE)
                return 1 + (size / SEGMENT_SIZE);
            else
                return size / SEGMENT_SIZE;
        }

        Uint64 getSize() const;

        const std::string &getName() const;

        const std::string &getPath() const;

        bool operator==(const Resource &other) const;

        bool operator!=(const Resource &other) const;
    private:
        Uint64 size;
        std::string name;
        /*atrribs not checked for equality*/
        std::string path;
        std::vector<Host *> host_in_possetion;

        friend class Resource_Database;
    };
}

#endif //SIMPLE_P2P_RESOURCE_H
