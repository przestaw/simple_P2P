/*
 * RequestWorker.h
 * Kamil Zacharczuk
 */
#ifndef SIMPLE_P2P_REQUEST_WORKER_H
#define SIMPLE_P2P_REQUEST_WORKER_H

#include <memory>	// enable_shared_from_this
#include <boost/asio.hpp>

#include "GeneralTypes.h"

using boost::asio::ip::tcp;

namespace SimpleP2P 
{
	/**
	 * \brief TCP connection handler, created by the TCP server.
	 *
	 * Receives the file request, buffers requested segments and sends them to the client.
	 */
	class RequestWorker
		: public std::enable_shared_from_this<RequestWorker>
	{
	public:
		/**
		 * Constructor allows setting the socket on which the connection is established
		 */
		RequestWorker (boost::asio::io_service& io_service);
		
		/**
		 * Start handling the request.
		 */
		void start();
		
		/** 
		 * Get socket.
		 */
		tcp::socket& socket();
	
	private:
		static const Uint16 MAX_RECVD_DATA_LENGHT = 400;
		
		tcp::socket _socket;	//!< Socket on which the connection is established.
		char recvd_data [MAX_RECVD_DATA_LENGHT];
		
		void handle_read(const boost::system::error_code& error, std::size_t bytes_transferred);
		
	};
}

#endif // SIMPLE_P2P_REQUEST_WORKER_H
