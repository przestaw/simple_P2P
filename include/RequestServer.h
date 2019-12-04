/**
 * RequestServer.h
 * Kamil Zacharczuk
 */
#ifndef SIMPLE_P2P_REQUEST_SERVER_H
#define SIMPLE_P2P_REQUEST_SERVER_H

#include <memory>	// make_shared
#include <utility>	// move
#include <boost/asio.hpp>

#include "RequestWorker.h"

using boost::asio::ip::tcp;

namespace SimpleP2P {
	class RequestServer {
	private:
		tcp::acceptor _acceptor;
		
		void accept_connection() {
			_acceptor.async_accept(
				// Accept handler.
				[this] (boost::system::error_code err, tcp::socket socket) {
					if (!err){
						std::make_shared<RequestWorker>(std::move(socket))->start();
					}
					
					accept_connection(); // Wait for another request.
				});
			}
	
	public:
		RequestServer(boost::asio::io_context& io_context, short port)
			: _acceptor(io_context, tcp::endpoint(tcp::v4(), port)) {
				accept_connection();
		}
	}
}
						
#endif // SIMPLE_P2P_REQUEST_SERVER_H	

