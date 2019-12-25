/*
 * FileManager.cpp
 * Kamil Zacharczuk
 */
#include <cstddef> // size_t
#include <string>
#include <fstream>

#include "FileRequest.h"
#include "FileManager.h"
#include "GeneralTypes.h"

#define FILE_NAME_LENGHT 256
#define SEGMENT_SIZE 1024

// TODO: synchronizing access to the files.

namespace SimpleP2P {
	FileManager::FileManager()
	{}

	void FileManager::get_file(FileRequest request, char* result, std::size_t size) {
		char file_name[FILE_NAME_LENGHT];
		/*std::string resource_header = request.get_resource_header();
		resource_header.copy(file_name, FILE_NAME_LENGHT, 0);*/
		// TODO: get file name from the FileRequest object
		
		std::vector<Uint32> segments = request.get_segments(); // Numbers of requested segments.

		// Open the file.
		std::ifstream file(file_name, std::ifstream::in | std::ifstream::binary);
		
		if (file.fail()) {
			// TODO: log that file could not be open.
			return;
		}

		char buffer[SEGMENT_SIZE]; // Buffer for reading a single segment.

		for (Uint32 s : segments) {
			file.seekg(s * SEGMENT_SIZE); // Set the position of reading to the position of the requested segment.
			if (file.tellg() != s * SEGMENT_SIZE) {
				// TODO: log error.
			}

			// TODO: checking if we didn't overflow the 'result' array.
			
			file.read(buffer, SEGMENT_SIZE); // Read the contents of the segment.
			if (!file) {
				if (file.eof() && !file.fail()) {
					// We just read over EOF, that's ok - apparently we were reading the last segment.
					file.clear(); // Reset the state so we can continue reading next segments.
				}
				else {
					// TODO: log error
				}
			}
		}

		file.close();
	}

	void store_resource(CompleteResource& resource)
	{
		std::string file_name; // TODO: get file name from the CompleteResource object.

		// Open (create) the file in the mode of overwriting its content if the file exists.
		std::ofstream file(file_name, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);

		if (file.fail()) {
			// TODO: log error.
		}

		char* file_contents; // TODO: get file contents from the CompleteResource object.
		std::size_t file_size; // TODO: get file size from the CompleteResource object.

		file.seekp(0); // Write at the beggining of the file. Is this step necessary as the 'trunc' flag is set?
		file.write(file_contents, file_size);

		if (!file) {
			// TODO: log error.
		}

		file.close();
	}
}
