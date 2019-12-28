/*
 * FileManager.cpp
 * Kamil Zacharczuk
 */
#include <cstddef> // size_t
#include <string>
#include <fstream>
#include <mutex>
#include <condition_variable>

#include "SegmentRequest.h"
#include "FileManager.h"
#include "GeneralTypes.h"

namespace SimpleP2P {
	FileManager::~FileManager()
	{
		for (OpenFile* f : rlocked_files)
		{
			if (f != nullptr)
			{
				delete f;
			}
		}
		
		rlocked_files.erase(rlocked_files.begin(), rlocked_files.end());
		wlocked_files.erase(wlocked_files.begin(), wlocked_files.end());
	}
	
	bool FileManager::get_segment(const SegmentRequest request, char* result, const std::size_t requested_segment_size)
	{
		if (result == nullptr)
		{
			// TODO: log error
			return false;
		}
		if (requested_segment_size > SEGMENT_SIZE)
		{
			// TODO: log error (maximum segment size is defined in SEGMENT_SIZE [the only smaller segment can be the last one])
			return false;
		}
		
		OpenFile* file_ptr;
		
		rlmutex.lock();
		for (OpenFile* f : rlocked_files)
		{
			if (f->file_name == request.get_file_name())
			{
				file_ptr = f;
				break;
			}
		}
		rlmutex.unlock();
		
		/* Only proceed if this file has been opened by calling read_lock() */
		if (!file_ptr->stream.is_open())
		{
			// TODO: log that the file ought to have been opened by calling read_lock() before an attempt to read it.
			return false;
		}
		
		Uint32 segment = request.get_segment();
		
		file_ptr->stream.seekg(segment * SEGMENT_SIZE); // Set the position of reading to the position of the requested segment.
		if (file_ptr->stream.tellg() != segment * SEGMENT_SIZE)
		{	
			// TODO: log error.
			return false;
		}
		
		file_ptr->stream.read(result, requested_segment_size);	// Put requested number of bytes in the provided buffer.
		if (!file_ptr->stream)
		{
			if (file_ptr->stream.eof() && !file_ptr->stream.fail())
			{
				// We read over the eof - it is NOT a proper situation, but we can put to the logs something
				// like 'did you try to read the last segment and forgot that its size is smaller than usual?'
				// TODO: log error
			}
			else
			{
				// TODO: log error
				return false;
			}
		}
		
		return true;
	}
			
	void FileManager::store_resource(const CompleteResource& resource)
	{
		std::string file_name; // TODO: get file_name from the CompleteResource object.

		write_lock(file_name);
		
		// Open (create) the file in the mode of overwriting its content if the file exists.
		std::ofstream file(file_name, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);

		if (file.fail()) 
		{
			// TODO: log error.
		}

		char* file_contents; // TODO: get file contents from the CompleteResource object.
		std::size_t file_size; // TODO: get file size from the CompleteResource object.

		file.seekp(0); // Write at the beggining of the file. Is this step necessary as the 'trunc' flag is set?
		file.write(file_contents, file_size);

		if (!file) 
		{
			// TODO: log error.
		}

		file.close();
		write_unlock(file_name);
	}
	
	bool FileManager::read_lock (const std::string file_name)
	{
		// Check if the file is wlocked
		bool is_locked = true;
		wlmutex.lock();
		while (is_locked)
		{
			for (auto wlocked : wlocked_files)
			{
				if (wlocked == file_name)
				{
					// File is wlocked - we must block.
					break;
				}
			}
			if (is_locked)
			{
				wlmutex.unlock();
				{
					std::unique_lock<std::mutex> lk (condvmutex);
					condv.wait(lk);
				}
				wlmutex.lock();
			}
		}
		wlmutex.unlock();
		
		// Check if the file is rlocked
		rlmutex.lock();
		for (OpenFile* f : rlocked_files)
		{
			if (f->file_name == file_name)
			{
				// File already rlocked - ok, multiple rlocks are permitted.
				rlmutex.unlock();
				return true;
			}
		}
		
		/* File not rlocked nor wlocked - perform rlock. */
		OpenFile* file_ptr = new OpenFile;
		file_ptr->file_name = file_name;
		file_ptr->stream.open(file_name, std::ifstream::in | std::ifstream::binary);
		
		if (file_ptr->stream.fail())
		{
			// TODO: log that a file could not be opened
			rlmutex.unlock();
			return false;
		}
		
		rlocked_files.push_back(file_ptr);
		
		rlmutex.unlock();
		return true;
	}
	
	void FileManager::read_unlock (const std::string file_name)
	{
		rlmutex.lock();
		for (auto it = rlocked_files.begin(); it != rlocked_files.end(); ++it)
		{
			if (file_name == (*it)->file_name)
			{
				(*it)->stream.close();
				delete *it;
				rlocked_files.erase(it);
			}
		}
		rlmutex.unlock();
	}
	
	void FileManager::write_lock (const std::string file_name)
	{
		wlmutex.lock();
		
		// Check if the file is rlocked
		bool is_locked = true;
		rlmutex.lock();
		while (is_locked)
		{		
			for (OpenFile* f : rlocked_files)
			{
				if (file_name == f->file_name)
				{
					// The file is rlocked.
					break;
				} 
				
				is_locked = false;
			}
			
			if (is_locked)
			{
				rlmutex.unlock();
				wlmutex.unlock();
				{
					std::unique_lock<std::mutex> lk (condvmutex);
					condv.wait(lk);
				}
				wlmutex.lock();
				rlmutex.lock();
			}
		}
		rlmutex.unlock();
		
		// !!
		// We are not checking if the file is wlocked, because only one thread will perform wlocks (see Downloader)
		// !!
		
		// Perform wlock
		wlocked_files.push_back(file_name);	
		
		wlmutex.unlock();
	}
	
	void FileManager::write_unlock(const std::string file_name)
	{
		wlmutex.lock();
		for (auto it = wlocked_files.begin(); it != wlocked_files.end(); ++it)
		{
			if (*it == file_name)
			{
				wlocked_files.erase(it);
				wlmutex.unlock();
				{
					std::lock_guard<std::mutex> lk (condvmutex);
					condv.notify_all();
				}
				return;
			}
		}
		wlmutex.unlock();
	}						
}
