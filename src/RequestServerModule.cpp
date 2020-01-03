/* 
 * RequestServerModule.cpp
 * Kamil Zacharczuk
 */
#include <boost/asio.hpp>
#include <thread>
#include "RequestServerModule.h"
#include "RequestServer.h"

namespace SimpleP2P {
	RequestServerModule::RequestServerModule(Uint16 port_)
		: port(port_)
	{}
	
	std::thread RequestServerModule::init() {
		/// The thread of the server.
		std::thread res;
		
		try {
			boost::asio::io_service io_service;
			RequestServer server (io_service, port);
			res = server.init();
			io_service.run();
		}
		catch (std::exception& e) {
			// TODO: logging module
		}
		
		return res;
	}
}
