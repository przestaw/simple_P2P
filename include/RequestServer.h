/**
 * RequestServer.h
 * Kamil Zacharczuk
 */
#ifndef SIMPLE_P2P_REQUEST_SERVER_H
#define SIMPLE_P2P_REQUEST_SERVER_H

#include "RequestWorker.h"

using boost::asio::ip::tcp;

namespace SimpleP2P {
	class RequestServer {
	private:
		tcp::acceptor _acceptor;
		
		void accept_connection();
	public:
		RequestServer(boost::asio::io_context& io_context, short port);
}
						
#endif // SIMPLE_P2P_REQUEST_SERVER_H	

