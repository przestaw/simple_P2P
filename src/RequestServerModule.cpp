/* 
 * RequestServerModule.cpp
 * Kamil Zacharczuk
 */
#include <boost/asio.hpp>
#include "RequestServer.h"

namespace SimpleP2P {
	RequestServerModule::RequestServerModule(short port_)
		: port(port_)
	{}
	
	std::thread RequestServerModule::init() {
		/// The thread of the server.
		std::thread res;
		
		try {
			boost::asio::io_context io_context;
			RequestServer server (io_context, port);
			res = server.init();
			io_context.run();
		}
		catch (Exception e) {
			// TODO: logging module
		}
		
		return res;
	}
}
