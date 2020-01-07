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
	RequestWorker::RequestWorker (boost::asio::io_service& io_service, FileManager& fm)
		: _socket(io_service), file_manager(fm)
	{
	
	}
	
	RequestWorker::~RequestWorker ()
	{
		if (send_data != nullptr)
		{
			delete send_data;
		}
	}
	
	void RequestWorker::start() 
	{
		send_data = (Uint8*) malloc (SEGMENT_SIZE);
			
		if (send_data == nullptr)
		{
			// TODO: log error.
			
			throw std::exception();
		}
		
		_socket.async_read_some(boost::asio::buffer(recv_data, MAX_RECV_DATA_LENGHT),
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
		 	Uint8 command = recv_data[0];
			
			// Get the file_name from the recv_data buffer (it's null-terminated)
			auto end = std::find (recv_data+1, recv_data+1+FILE_NAME_LENGHT, '\0');
			std::string file_name((recv_data+1), end);
					
			if (command == REQ_SEGMENT)
			{
				Uint64 file_size;
				std::copy(recv_data+1+FILE_NAME_LENGHT, recv_data+1+FILE_NAME_LENGHT+FILE_SIZE_LENGHT, &file_size);
				file_size = ntohs(file_size);
				
				Uint16 segment;
				std::copy(recv_data+1+FILE_NAME_LENGHT+FILE_SIZE_LENGHT, recv_data+1+FILE_NAME_LENGHT+FILE_SIZE_LENGHT+sizeof(Uint16), &segment);
				segment = ntohs(segment);
				
				Uint32 requested_segment_size;
				double nsegments = (double)file_size / SEGMENT_SIZE;
			
				// Check if the requested segment is the last segment of the file (and its size != SEGMENT_SIZE)
				// and if this is the case calculate its size.
				if (segment == (Uint64)nsegments+1)
				{
					requested_segment_size = file_size - (Uint64)nsegments * SEGMENT_SIZE;
				}
				else
				{
					requested_segment_size = SEGMENT_SIZE;
				}
				
				/* Explicit block in which 'send_data' is declared */ 
				//{
					//char* send_data = (char*) malloc (SEGMENT_SIZE);	// If the requested segment is the last segment and it's shorter than SEGMENT_SIZE,
																		// the buffer will be complemented with 0's. 
					if (send_data == nullptr)
					{
						// TODO: log error and do sth.
					}
		
					file_manager.read_lock(file_name);
					
					if (!file_manager.get_segment(SegmentRequest(file_name, segment), send_data, requested_segment_size))
					{
						// TODO: log error and do sth.
					}
					
					// Send the segment.
					boost::asio::async_write(_socket,
										boost::asio::buffer(send_data, sizeof(send_data)),
										boost::bind(&RequestWorker::handle_write, this, boost::asio::placeholders::error));
					
				//}
			}
			else if (command == QUIT_CONN)
			{
				file_manager.read_unlock(file_name);
				
				delete this;
			}
		}
		else
		{
			delete this;
		}
	}
	
	void RequestWorker::handle_write(const boost::system::error_code& error)
	{
		if (!error)
		{
			// TODO: log that the segment was successfully sent.
			
			// Get back to listening for further requests.
			_socket.async_read_some(boost::asio::buffer(recv_data, MAX_RECV_DATA_LENGHT),
        						boost::bind(&RequestWorker::handle_read, this, 
        									boost::asio::placeholders::error, 
        									boost::asio::placeholders::bytes_transferred));
        }
        else
		{
			// TODO: log error.
			
			// Kill yourself, you can't even send a mere kilobyte.
			delete this;
		}
	}
}
