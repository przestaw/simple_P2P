/* 
 * RequestServerModule.cpp
 * Kamil Zacharczuk
 */
#include <boost/asio.hpp>
#include <thread>
#include "RequestServerModule.h"
#include "RequestServer.h"

namespace SimpleP2P {
	RequestServerModule::RequestServerModule(Uint16 port_, FileManager& fm)
		: port(port_), file_manager(fm)
	{}
	
	std::thread RequestServerModule::init() {
		/// The thread of the server.
		std::thread res;
		
		try {
			boost::asio::io_service io_service;
			RequestServer server (io_service, port, file_manager);
			res = server.init();
			io_service.run();
		}
		catch (std::exception& e) {
			// TODO: logging module
		}
		
		return res;
	}
}
