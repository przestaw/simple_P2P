/**
 * RequestWorker.h
 * Kamil Zacharczuk
 */
#ifndef SIMPLE_P2P_REQUEST_WORKER_H
#define SIMPLE_P2P_REQUEST_WORKER_H

#include <memory>	// enable_shared_from_this
#include <utility>	// move	
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace SimpleP2P {
	class RequestWorker
		: public std::enable_shared_from_this<RequestWorker> // Necessary, because copies of the worker will be made by the software as required.
	{
	private:
		tcp::socket _socket;
	
		// TODO: Actual work of the worker.
	
	public:
		RequestWorker (tcp::socket socket)
			: _socket(std::move(socket))
		{}
		
		void start() {
			// TODO: Do the work.
		}
	}
}

#endif // SIMPLE_P2P_REQUEST_WORKER_H
