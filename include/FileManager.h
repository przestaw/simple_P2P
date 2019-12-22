/*
 * FileManager.h
 * Kamil Zacharczuk
 */
#ifndef SIMPLE_P2P_FILE_MANAGER_H
#define SIMPLE_P2P_FILE_MANAGER_H

#include <string>
#include <fstream.h>

#include "FileRequest.h"

namespace SimpleP2P
{
	class FileManager
	{
	public:
		FileManager();
		~FileManager();
		
		void get_file (FileRequest request, char* result);  // Segments of the file - both specificated in the first parameter - will
															// be returned in the char array provided as the second parameter. 
		void store_resource (CompleteResource resource);
	private:
		// TODO: synchronizing access to the files.
	}
}

#endif // SIMPLE_P2P_FILE_MANAGER_H
