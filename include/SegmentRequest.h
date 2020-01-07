/*
 * SegmentRequest.h
 * Kamil Zacharczuk
 */
#ifndef SIMPLE_P2P_SEGMENT_REQUEST_H
#define SIMPLE_P2P_SEGMENT_REQUEST_H
 
#include <string>
#include <vector>

#include "GeneralTypes.h"

namespace simpleP2P 
{	
	/**
	 * \brief Carries info about a single resource transfer request - resource header and number of the segment to transfer.
	 *
	 * An instance of this class is created by TCP client, sent to TCP server, which passes it to the FileManager
	 * in order to get the requested segment of the requested file and send it to the TCP client.
	 */
	class SegmentRequest 
	{
	public:
		/**
		 * Constructor allows specificating the resource and the segment. The fields then can't be modified, only get.
		 */
		SegmentRequest(std::string f, Uint32 s);
		
		// -----------------
		// Getter functions.
		
		/**
		 * \brief Get the resource header.
		 */
		std::string get_file_name() const;
		/**
		 * \brief Get the number of the segment.
		 */
		Uint32 get_segment() const;
		
	private:
		const std::string file_name;	//!< Name of the requested file.
		const Uint32 segment;			//!< Number of the requested segment of the file.
	};
}

#endif // SIMPLE_P2P_SEGMENT_REQUEST_H
