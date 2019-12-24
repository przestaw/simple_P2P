/*
 * RequestServerModule.h
 * Kamil Zacharczuk
 */
#ifndef SIMPLE_P2P_REQUEST_SERVER_MODULE_H
#define SIMPLE_P2P_REQUEST_SERVER_MODULE_H

namespace SimpleP2P	
{
	/**
	 * Module of the TCP server receiving file requests and sending the requested files' segments.
	 */
	class RequestServerModule 
	{
	public:
		/**
		 * Constructor, allows setting the port for the server.
		 */
		RequestServerModule(short port_);
		
		/**
		 * \brief Returns the thread object for the module.
		 *
		 * Starts the server and returns the thread in which the server works. 
		 */
		std::thread init();
	
	private:
		short port;	//!< Port for the server.
	};
}			

#endif // SIMPLE_P2P_REQUEST_SERVER_MODULE_H		 
			
