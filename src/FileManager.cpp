/*
 * FileManager.cpp
 * Kamil Zacharczuk
 */
#include <cstddef> // size_t
#include <string>
#include <fstream>
#include <mutex>
#include <condition_variable>
#include <sstream> // stringstream
#include "SegmentRequest.h"
#include "FileManager.h"
#include "CompleteResource.h"
#include "GeneralTypes.h"

namespace simpleP2P {
	FileManager::FileManager(Logging_Module& lm)
		: logging_module(lm)
	{}

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
	
	bool FileManager::get_segment(const SegmentRequest request, Uint8* result, const std::size_t requested_segment_size)
	{
		if (result == nullptr)
		{
			logging_module.add_log_line("FileManager: ptr to 'result' buffer passed as parameter is null!\n Segment reading FAILED\n",
			                               std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
			return false;
		}
		if (requested_segment_size > SEGMENT_SIZE)
		{
			logging_module.add_log_line("FileManager: requested segment size > typical segment size (must be <=)!\n Segment reading FAILED\n",
			                               std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
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
			logging_module.add_log_line("FileManager::get_segment(): file not rlocked! (Call read_lock() first)\n Segment reading FAILED\n",
			                                std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
			return false;
		}
		
		Uint32 segment = request.get_segment();
		
		file_ptr->stream.seekg(segment * SEGMENT_SIZE); // Set the position of reading to the position of the requested segment.
		if (file_ptr->stream.tellg() != segment * SEGMENT_SIZE)
		{	
			logging_module.add_log_line("FileManager: [BUG] Reading position not set to the begging of the requested segment!\n Segment reading FAILED\n",
			                                std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
			return false;
		}
		
		file_ptr->stream.read(reinterpret_cast<char*>(result), requested_segment_size);	// Put requested number of bytes in the provided buffer.
		if (!file_ptr->stream)
		{
			if (file_ptr->stream.eof() && !file_ptr->stream.fail())
			{
				// We read over the eof - it is NOT a proper situation, but we can distinguish it from other errors in the logs
				std::stringstream logmsg;
				logmsg << "FileManager: EOF read! (Did you try to read the last segment and forgot" << std::endl << "to calculate its size?)" 
								<< "Segment reading FAILED" << std::endl;
				logging_module.add_log_line(logmsg.str(), std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
				
			}
			else
			{
				logging_module.add_log_line("FileManager: ERROR reading the segment from the physical file!\n",
				                                std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
				return false;
			}
		}

		logging_module.add_log_line("FileManager: segment successfully read from the physical file\n",
				                                std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));

		// If we are reading the last segment (and its size < SEGMENT_SIZE), complement the result buffer with 0's.
		// (the result buffer's size always == SEGMENT_SIZE)
		if (requested_segment_size < SEGMENT_SIZE)
		{
			logging_module.add_log_line("FileManager: [note] requested segment size < typical segment size. It must be the last segment of the file\n",
			                                std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));			
			std::fill (result+requested_segment_size, result+SEGMENT_SIZE, '\0');
		}

		return true;
	}
			
	void FileManager::store_resource(CompleteResource& complete_resource)
	{
		char c_file_name [FILE_NAME_LENGHT];
		std::vector<Uint8> resource_header = complete_resource.get_resource()->generate_resource_header();
		std::copy(resource_header.begin(), resource_header.begin()+FILE_NAME_LENGHT, c_file_name);

		auto end = std::find (c_file_name, c_file_name+FILE_NAME_LENGHT, '\0');

		std::string file_name (c_file_name, end);

		write_lock(file_name);
		
		// Open (create) the file in the mode of overwriting its content if the file exists.
		std::ofstream file(file_name, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);

		if (file.fail()) 
		{
			std::stringstream logmsg;
			logmsg << "FileManager: opening file " << file_name << " for writing (creating it) FAILED!" << std::endl << "File storing FAILED" << std::endl;
			logging_module.add_log_line(logmsg.str(), std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
			return;
		}

		Uint8* file_contents = complete_resource.get_data();
	
		size_t file_size;
		std::copy (resource_header.begin()+FILE_NAME_LENGHT, resource_header.end(), &file_size);

		file.seekp(0); // Write at the beggining of the file. Is this step necessary as the 'trunc' flag is set?
		file.write(reinterpret_cast<char*>(file_contents), file_size);

		if (!file) 
		{
			std::stringstream logmsg;
			logmsg << "FileManager: ERROR storing the file " << file_name << " on disc!" << std::endl;
			logging_module.add_log_line(logmsg.str(), std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
		}
		else
		{
			std::stringstream logmsg;
			logmsg << "FileManager: file " << file_name << " successfully stored on disc" << std::endl;
			logging_module.add_log_line(logmsg.str(), std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
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
			rlmutex.unlock();
			std::stringstream logmsg;
			logmsg << "FileManager: ERROR opening file " << file_name << " for reading!" << std::endl;
			logging_module.add_log_line(logmsg.str(), std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
			return false;
		}
		
		rlocked_files.push_back(file_ptr);
		
		rlmutex.unlock();
		std::stringstream logmsg;
		logmsg << "FileManager: file " << file_name << " successfully opened for reading" << std::endl;
		logging_module.add_log_line(logmsg.str(), std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
		return true;
	}
	
	void FileManager::read_unlock (const std::string file_name)
	{
		bool erased = false;
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

		std::stringstream logmsg;
		if (erased)
		{
				logmsg << "FileManager: file " << file_name << " successfully closed for reading" << std::endl;
		}
		else
		{
				logmsg << "FileManager: could not close file " << file_name << " for reading as it was not opened for reading!" << std::endl;
		}

		logging_module.add_log_line(logmsg.str(), std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
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
		// We are not checking if the file is wlocked, because only one thread will perform wlocks (DownloadWorker)
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
