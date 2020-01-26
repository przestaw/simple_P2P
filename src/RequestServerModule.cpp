/*
 * RequestServerModule.cpp
 * Kamil Zacharczuk
 */
#include "RequestServerModule.h"
#include "RequestServer.h"
#include <boost/asio.hpp>
#include <thread>
#include <sstream> // stringstream

namespace simpleP2P {
    RequestServerModule::RequestServerModule(Uint16 port_, FileManager &fm, Logging_Module &lm)
            : port(port_), file_manager(fm), logging_module(lm) {}

    std::thread RequestServerModule::init() {
        // The thread of the server.
        std::thread res;

        try {
            boost::asio::io_service io_service;
            RequestServer server(io_service, port, file_manager, logging_module);
            res = server.init();
            io_service.run();

            logging_module.add_log_line("RequestServerModule successfully initialized",
                                        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
        }
        catch (std::exception &e) {
            std::stringstream logmsg;
            logmsg << "RequestServerModule: caught exception while initializing:\n" << e.what();
            logging_module.add_log_line(logmsg.str(),
                                        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
        }

        return res;
    }
}
