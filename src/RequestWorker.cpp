/*
 * RequestWorker.cpp
 * Kamil Zacharczuk
 */
#include <cstddef> // size_t
#include <utility> // move
#include <algorithm> // find	
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "RequestWorker.h"
#include "GeneralTypes.h"

using boost::asio::ip::tcp;

namespace SimpleP2P {
	RequestWorker::RequestWorker (boost::asio::io_service& io_service)
		: _socket(io_service)
	{}
	
	void RequestWorker::start() 
	{
		_socket.async_read_some(boost::asio::buffer(recvd_data, MAX_RECVD_DATA_LENGHT),
        						boost::bind(&RequestWorker::handle_read, this, 
        									boost::asio::placeholders::error, 
        									boost::asio::placeholders::bytes_transferred));     	
	}
		
	tcp::socket& RequestWorker::socket()
	{	
		return _socket;
	}
	
	void RequestWorker::handle_read(const boost::system::error_code& error,
      std::size_t bytes_transferred)
	{
		if (!error)
		{
			// Get the command
			char command = recvd_data[0];
			
			// Get the file_name from the recvd_data buffer (it's null-terminated)
			char* end = std::find (recvd_data+1, recvd_data+1+255, '\0');
			std::string file_name(recvd_data+1, end);
					
			switch (command)
			{
			case REQ_SEGMENT:
/*
				str_resource_header.copy(recvd_data, FILE_NAME_LENGHT + 64, 1);
				std::vector<Int8> resource_header; // TODO: parse str_resource_header to resource_header
*/				
				Uint32 segment; // TODO: get segment number from recv_data
				
				Uint32 requested_segment_size; 
				// TODO: check if the requested segment is the last segment of the file and if this is the case calculate its size
				if (/*check here*/ true)
				{
					requested_segment_size = SEGMENT_SIZE;
				}
				else
				{
					/* calculate here */
				}
				
				
				/* Explicit block in which 'buffer' is declared */ 
				{
					char* buffer = (char*) malloc (requested_segment_size);
					
					if (buffer == nullptr)
					{
						// TODO: log error and do sth.
					}

/*  				TO BE IMPLEMENTED (file_manager must be passed in parameter or FileManager must be static)
					-----------------			
					file_manager.read_lock(file_name);
					
					if (!file_manager.get_segment(SegmentRequest(file_name, segment), buffer, requested_segment_size))
					{
						// TODO: log error and do sth.
					}
					
					// TODO: async write the segment
*/
				  			
				  	delete buffer;	
					break;
				}
			case QUIT_CONN:
			
/*				TO BE IMPLEMENTED (file_manager must be passed in parameter or FileManager must be static)
				-----------------
				file_manager.read_unlock(file_name);
*/
				delete this; // (?)
			}
		}
		else
		{
			delete this;
		}
	}
}
