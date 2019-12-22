#include <cstddef> // size_t
#include <string>
#include <fstream.h>

#include "FileRequest.h"
#include "FileManager.h"

#define FILE_NAME_LENGHT 256
#define SEGMENT_SIZE 1024

// TODO: synchronizing access to the files.

namespace SimpleP2P {
	FileManager::FileManager()
	{}

	// CAUTION. All segments will be returned concatenated in a single char array, provided as the parameter.
	// They will be put to the array in the order provided with FileRequest object.
	// If you requested the last segment of the file and it's shorter than SEGMENT_SIZE
	// make sure you will be able to localize it in the array.
	void FileManager::get_file(FileRequest request, char* result) {
		char file_name[FILE_NAME_LENGHT];
		std::string resource_header = request.get_resource_header();
		resource_header.copy(file_name, FILE_NAME_LENGHT, 0);

		std::vector<short> segments = request.get_segments();

		// Open the file.
		std::ifstream file(file_name, std::ifstream::in | std::ifstream::binary);
		
		if (file.fail()) {
			// TODO: log that file could not be open.
			return nullptr;
		}

		char buffer[SEGMENT_SIZE];

		for (long s : segments) {
			file.seekg(s * SEGMENT_SIZE); // Set the position of reading to the requested segment position.
			if (file.tellg() != s * SEGMENT_SIZE) {
				// TODO: log error.
			}

			file.read(buffer, SEGMENT_SIZE);
			if (!file) {
				if (file.eof && !file.fail()) {
					// We just read over EOF - apparently we were reading the last segment.
					file.clear(); // Reset the state so we can continue reading.
				}
				else {
					// TODO: log error
				}
			}
		}

		file.close();

		return result;
	}

	void store_resource(CompleteResource resource)
	{
		std::string file_name; // TODO: get file name from the CompleteResource object.

		std::ofstream file(file_name, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);

		if (file.fail()) {
			// TODO: log error.
		}

		char* file_contents; // TODO: get file contents from the CompleteResource object.
		std::size_t file_size; // TODO: get file size from the CompleteResource object.

		file.seekp(0); // Write at the beggining of the file. Is this necessary if the flag trunc is set?
		file.write(file_contents, file_size);

		if (!file) {
			// TODO: log error.
		}

		file.close();
	}
}