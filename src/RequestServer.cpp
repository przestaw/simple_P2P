/* 
 * RequestServer.cpp
 * Kamil Zacharczuk
 */
 
#include <memory>	// make_shared
#include <utility>	// move
#include <thread>	// thread
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "RequestServer.h"
#include "GeneralTypes.h"

using boost::asio::ip::tcp;

namespace SimpleP2P
{
	RequestServer::RequestServer (boost::asio::io_service& _io_service, Uint16 port)
		: io_service(_io_service), acceptor(_io_service, tcp::endpoint(tcp::v4(), port))
	{}

	std::thread RequestServer::init() 
	{
		return std::thread([=] { start_accept(); });
	}
	
	void RequestServer::start_accept()
	{
		RequestWorker* new_worker = new RequestWorker(io_service);
		
    	acceptor.async_accept(new_worker->socket(),
			boost::bind(&RequestServer::handle_accept, this, new_worker, boost::asio::placeholders::error));
	}
	
	void RequestServer::handle_accept(RequestWorker* new_worker, const boost::system::error_code& error)
	{
	    if (!error)
	    {
	      new_worker->start();
	    }
	    else
	    {
	      delete new_worker;
	    }
	
	    start_accept();
	}
	
	/*
	void RequestServer::accept_connection() 
	{
		_acceptor.async_accept(
			// Accept handler.
			[this] (boost::system::error_code err, tcp::socket socket) 
			{
				if (!err)
				{
					std::make_shared<RequestWorker>(std::move(socket))->start();
				}
				
				accept_connection(); // Wait for another request.
			});
	}*/
}
