/**
 * RequestWorker.cpp
 * Kamil Zacharczuk
 */
#include <utility>	// move	
#include <boost/asio.hpp>
#include "RequestWorker.h"

using boost::asio::ip::tcp;

namespace SimpleP2P {
	RequestWorker::RequestWorker (tcp::socket socket)
		: _socket(std::move(socket))
	{}
	
	void RequestWorker::start() {
		// TODO: Do the work.
	}
	
	// TODO: The actual work.
}
