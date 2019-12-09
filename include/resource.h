//
// Created by przemek on 09.12.2019.
//

#ifndef SIMPLE_P2P_RESOURCE_H
#define SIMPLE_P2P_RESOURCE_H

#include <vector>
#include <string>
#include <GeneralTypes.h>

namespace simpleP2P {
    class Host;

    class Resource { //!< contains file information and points to nodes with file possesion
    public:
        Resource(std::string resource_header);

        std::string generate_resource_header();

        bool add_host(Host host);

        bool has_host(Host host);

    private:
        Uint64 size;
        Uint16 segments;
        std::string name;
        std::string path;
        //FIXME: Where to put file from parts ? ? ?
        //TODO: file_ptr?, array of segments?, bitset of segments stat?
        /*atrribs not checked for equality*/
        std::vector<Host> host_in_possetion;
    };
}

#endif //SIMPLE_P2P_RESOURCE_H
