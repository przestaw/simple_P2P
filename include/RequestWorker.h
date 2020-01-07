/*
 * RequestWorker.h
 * Kamil Zacharczuk
 */
#ifndef SIMPLE_P2P_REQUEST_WORKER_H
#define SIMPLE_P2P_REQUEST_WORKER_H

#include <memory>	// enable_shared_from_this
#include <boost/asio.hpp>

#include "FileManager.h"
#include "GeneralTypes.h"

using boost::asio::ip::tcp;

namespace simpleP2P 
{
	/**
	 * \brief TCP connection handler, created by the TCP server.
	 *
	 * Receives the file request, buffers requested segments and sends them to the client.
	 */
	class RequestWorker
		//: public std::enable_shared_from_this<RequestWorker>
	{
	public:
		/**
		 * Constructor allows setting the socket on which the connection is established
		 */
		RequestWorker (boost::asio::io_service& io_service, FileManager& fm);
		
		~RequestWorker();
		
		/**
		 * Start handling the request.
		 */
		void start();
		
		/** 
		 * Get socket.
		 */
		tcp::socket& socket();
	
	private:
		static const Uint16 MAX_RECV_DATA_LENGHT = 300;	// A little more than what we will ever expect.
		
		tcp::socket _socket;		//!< Socket on which the connection is established.
		FileManager& file_manager;	//!< FileManager for accessing requested files.
		
		Uint8 recv_data [MAX_RECV_DATA_LENGHT];
		Uint8* send_data;
		
		void handle_read(const boost::system::error_code& error, std::size_t bytes_transferred);
		void handle_write(const boost::system::error_code& error);
		
	};
}

#endif // SIMPLE_P2P_REQUEST_WORKER_H
