/** 
 * RequestServerModule.cpp
 * Kamil Zacharczuk
 */
#include <boost/asio.hpp>
#include "RequestServer.h"

namespace SimpleP2P {
	RequestServerModule::RequestServerModule(short port) {
		try{
			boost::asio::io_context io_context;
			RequestServer server (io_context, port); // Start the server.
			io_context.run();
		}
		catch (Exception e){
			std::err << "Exception (in RequestServerModule()): " << e.what() << std::endl;
		}
	}
}
