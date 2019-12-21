/**
 * FileRequest.h
 * Kamil Zacharczuk
 */
#ifndef SIMPLE_P2P_FILE_REQUEST_H
#define SIMPLE_P2P_FILE_REQUEST_H
 
#include <string>
#include <vector>
#include <initializer_list>

namespace SimpleP2P {
	class FileRequest {
	public:
		FileRequest(std::string rh, std::initializer_list<short> s);
		~FileRequest();
		
		/* Getters */
		std::string get_resource_header() const;
		std::vector<short> get_segments() const;
	private:
		const std::string resource_header;
		const std::vector<short> segments;
	}
}

#endif // SIMPLE_P2P_FILE_REQUEST_H
