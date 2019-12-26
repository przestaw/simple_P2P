/*
 * FileRequest.h
 * Kamil Zacharczuk
 */
#ifndef SIMPLE_P2P_FILE_REQUEST_H
#define SIMPLE_P2P_FILE_REQUEST_H
 
#include <string>
#include <vector>
#include <initializer_list>

#include "GeneralTypes.h"

namespace SimpleP2P 
{
	class CompleteResource;
	
	/**
	 * \brief Carries info about a single file transfer request - resource header and numbers of wanted segments.
	 *
	 * An instance of this class is created by TCP client, sent to TCP server, which passes it to the FileManager
	 * in order to get the requested segments of the requested file and send them to the TCP client.
	 */
	class FileRequest 
	{
	public:
		/**
		 * Constructor allows specificating the resource and segments. The fields then can't be modified, only get.
		 */
		FileRequest(std::vector<Int8> rh, std::initializer_list<Uint32> s);
		
		// -----------------
		// Getter functions.
		
		/**
		 * \brief Get the resource header.
		 */
		std::vector<Int8> get_resource_header() const;
		/**
		 * \brief Get the segments' numbers.
		 */
		std::vector<Uint32> get_segments() const;
		
	private:
		const std::vector<Int8> resource_header;	//!< Header of requested file.
		const std::vector<Uint32> segments;			//!< Numbers of requested segments of the file.
	};
}

#endif // SIMPLE_P2P_FILE_REQUEST_H
