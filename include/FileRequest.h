/**
 * FileRequest.h
 * Kamil Zacharczuk
 */
#ifndef SIMPLE_P2P_FILE_REQUEST_H
#define SIMPLE_P2P_FILE_REQUEST_H
 
#include <string>

namespace SimpleP2P {
	class FileRequest {
	private:
		const std::string resource_header;
		const short first_seg;	// No. of the 1st file segment to transfer.
		const short last_seg;	// No. of the last file segment to transfer.
		
	public:
		FileRequest(std::string f, short s);				// Single segment request.
		FileRequest(std::string f, short fs, short ls);		// Multi segment request.
		~FileRequest();
		
		/* Getters */
		std::string resource_header() const;
		short first_segment() const;
		short last_segment() const;
	}
}

#endif // SIMPLE_P2P_FILE_REQUEST_H
