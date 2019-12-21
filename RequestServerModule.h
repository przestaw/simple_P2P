/**
 * RequestServerModule.h
 * Kamil Zacharczuk
 */
#ifndef SIMPLE_P2P_REQUEST_SERVER_MODULE_H
#define SIMPLE_P2P_REQUEST_SERVER_MODULE_H

namespace SimpleP2P	{
	class RequestServerModule {
	public:
		RequestServerModule(short port_);
		
		std::thread init();
	
	private:
		short port;
	}
}			

#endif // SIMPLE_P2P_REQUEST_SERVER_MODULE_H		 
			
