/*
 * RequestWorker.cpp
 * Kamil Zacharczuk
 */
#include <utility>	// move	
#include <boost/asio.hpp>
#include "RequestWorker.h"

using boost::asio::ip::tcp;

namespace SimpleP2P {
	RequestWorker::RequestWorker (boost::asio::io_service& io_service)
		: _socket(io_service)
	{}
	
	void RequestWorker::start() 
	{
		// TODO: Do the work.
	}
		
	tcp::socket& RequestWorker::socket()
	{	
		return _socket;
	}
	
	// TODO: The actual work.
}
