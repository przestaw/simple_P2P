/*
 * RequestServer.h
 * Kamil Zacharczuk
 */
#ifndef SIMPLE_P2P_REQUEST_SERVER_H
#define SIMPLE_P2P_REQUEST_SERVER_H

#include <boost/asio.hpp>
#include <thread>
#include "RequestWorker.h"
#include "GeneralTypes.h"

using boost::asio::ip::tcp;

namespace SimpleP2P 
{
	/**
	 * Asynchronous TCP server. It accepts connections asynchronously and for each of them creates a worker object to handle it.
	 */
	class RequestServer 
	{
	public:
		/**
		 * Constructor allows setting the parameters for the connnetion acceptor.
		 *
		 * @param io_context boost::asio::io_service for the acceptor.
		 * @param port Port for the acceptor to listen on.
		 */
		RequestServer(boost::asio::io_service& io_service, Uint16 port);
		
		/**
		 * Turns on the listening and accepting connections and returns the thread in which it works.
		 */
		std::thread init();
	private:
		boost::asio::io_service& io_service;
		tcp::acceptor acceptor;	//!< The acceptor listening and accepting connections.
		
		/**
		 * \brief Asynchronous connection accepting function.
		 * 
		 * When a connection is requested, it accepts it, creates a worker object to handle it
		 * and calls itself to continue listening.
		 */
		void start_accept();
		void handle_accept(RequestWorker* new_worker, const boost::system::error_code& error);
	};
}
						
#endif // SIMPLE_P2P_REQUEST_SERVER_H	

