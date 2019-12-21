/** 
 * RequestServer.h
 * Kamil Zacharczuk
 */
 
#include <memory>	// make_shared
#include <utility>	// move
#include <boost/asio.hpp>
#include "RequestServer.h"

namespace SimpleP2P {
	RequestServer::RequestServer (boost::asio::io_context& io_context, short port)
		: _acceptor(io_context, tcp::endpoint(tcp::v4(), port))
	{}

	std::thread RequestServer::init() {
		return std::thread([=] { accept_connection(); });
	}
	
	void RequestServer::accept_connection() {
		_acceptor.async_accept(
			// Accept handler.
			[this] (boost::system::error_code err, tcp::socket socket) {
				if (!err){
					std::make_shared<RequestWorker>(std::move(socket))->start();
				}
				
				accept_connection(); // Wait for another request.
			});
	}
}
