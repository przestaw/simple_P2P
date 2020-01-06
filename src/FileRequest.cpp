/*
 * FileRequest.cpp
 * Kamil Zacharczuk
 */
#include <string>
#include <vector>
#include <initializer_list>
#include "FileRequest.h"
#include "GeneralTypes.h"

namespace simpleP2P {
    FileRequest::FileRequest(std::vector<Int8> rh, std::initializer_list<Uint32> s)
            : resource_header(rh), segments(s) {}

    std::vector<Int8> FileRequest::get_resource_header() const {
        return resource_header;
    }

    std::vector<Uint32> FileRequest::get_segments() const {
        return segments;
    }
}
