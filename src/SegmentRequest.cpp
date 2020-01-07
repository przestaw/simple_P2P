/*
 * SegmentRequest.cpp
 * Kamil Zacharczuk
 */
#include <string> 
#include <vector>
#include "SegmentRequest.h"
#include "GeneralTypes.h"

namespace simpleP2P {
	SegmentRequest::SegmentRequest(std::string f, Uint32 s)
		: file_name(f), segment(s)
	{}
	
	std::string SegmentRequest::get_file_name() const {
		return file_name;
	}
	
	Uint32 SegmentRequest::get_segment() const {
		return segment;
	}
}
