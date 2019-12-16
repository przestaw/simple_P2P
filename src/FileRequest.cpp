/**
 * FileRequest.cpp
 * Kamil Zacharczuk
 */
#include <string> 
#include "FileRequest.h"

namespace SimpleP2P {
	FileRequest::FileRequest(std::string f, short s)
		: file(f), first_seg(s), last_seg(s)
	{}
		
	FileRequest::FileRequest(std::string f, short fs, short ls)
		: file(f), first_seg(fs), last_seg(ls)
	{}
	
	FileRequest::~FileRequest()
	{}
	
	std::string FileRequest::resource_header() const {
		return resource_header;
	}
	
	short FileRequest::first_segment() const {
		return first_seg;
	}
	short FileRequest::last_segment() const {
		return last_seg;
	}			
}
