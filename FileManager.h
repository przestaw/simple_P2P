/*
 * FileManager.h
 * Kamil Zacharczuk
 */
#ifndef SIMPLE_P2P_FILE_MANAGER_H
#define SIMPLE_P2P_FILE_MANAGER_H

// TODO: includes

namespace SimpleP2P
{
	class FileManager
	{
	private:
		/* TODO */
	public:
		FileManager();
		~FileManager();
		CompleteResource get_resource (Resource resource);	// TODO: change resource related classes' names
		void store_resource (CompleteResource resource);
	}
}

#endif // SIMPLE_P2P_FILE_MANAGER_H
