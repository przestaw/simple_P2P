/**
 * RequestServerModule.h
 * Kamil Zacharczuk
 */
#ifndef SIMPLE_P2P_REQUEST_SERVER_MODULE_H
#define SIMPLE_P2P_REQUEST_SERVER_MODULE_H

#include <boost/asio.hpp>

#include "RequestServer.h"
#include "RequestWorker.h"

namespace SimpleP2P	{
	class RequestServerModule {
	public:
		RequestServerModule(short port) {
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
}			

#endif // SIMPLE_P2P_REQUEST_SERVER_MODULE_H		 
			
