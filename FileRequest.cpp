/**
 * FileRequest.cpp
 * Kamil Zacharczuk
 */
#include <string> 
#include <vector>
#include <initializer_list>
#include "FileRequest.h"

namespace SimpleP2P {
	FileRequest::FileRequest(std::string rh, std::initializer_list<short> s)
		: resource_header(rh), segments(s)
	{}
	
	FileRequest::~FileRequest()
	{}
	
	std::string FileRequest::get_resource_header() const {
		return resource_header;
	}
	
	std::vector<short> get_segments() const {
		return segments;
	}
}
