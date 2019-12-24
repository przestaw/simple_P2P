/*
 * FileRequest.cpp
 * Kamil Zacharczuk
 */
#include <string> 
#include <vector>
#include <initializer_list>
#include "FileRequest.h"
#include "GeneralTypes.h"

namespace SimpleP2P {
	FileRequest::FileRequest(std::vector<Int8> rh, std::initializer_list<long> s)
		: resource_header(rh), segments(s)
	{}
	
	std::vector<Int8> FileRequest::get_resource_header() const {
		return resource_header;
	}
	
	std::vector<long> FileRequest::get_segments() const {
		return segments;
	}
}
