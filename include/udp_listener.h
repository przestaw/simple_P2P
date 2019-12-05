//
// Created by przemek on 05.12.2019.
//

#ifndef SIMPLE_P2P_UDP_LISTENER_H
#define SIMPLE_P2P_UDP_LISTENER_H


namespace simpleP2P {

    class resource;     //!< contains file information and points to nodes with file possesion
    class node;         //!< contains node information and points to files it possess

    class UDP_Listener {
    public:
        UDP_Listener(boost::asio::ip::udp::socket socket);

        std::thread init();

        bool has_file(std::string resource_header); // consider identifier
        bool add_file(resource);

    private:
        void has_changed();                     // if data about files is stored elsewhere
        boost::asio::ip::udp::socket socket     //!< socket on which listens for file information

        /* Not sure where to store information about structure of network and files*/
        std::vector <resource> resources;
        std::vector <nodes> nodes;
    };
};

#endif //SIMPLE_P2P_UDP_LISTENER_H
