/*
 * FileManager.h
 * Kamil Zacharczuk
 */
#ifndef SIMPLE_P2P_FILE_MANAGER_H
#define SIMPLE_P2P_FILE_MANAGER_H

#include <fstream>
#include <string>

#include "FileRequest.h"
#include "GeneralTypes.h"

namespace simpleP2P {
/**
 * \brief Handles read/write to the files on disc.
 *
 * An API which provides:
 * - buffering contents of requested segments of a specificated local file,
 * - storing a complete, downloaded file physically on the local disc.
 * Ensures synchronization of those operations.
 */
    class FileManager {
    public:
        FileManager();

        /**
         * \brief Buffers specificated segments of the specificated file in the char
         * array.
         *
         * (!) All segments will be returned concatenated in a single char array,
         * provided in the 'result' parameter. They will be put to the array in the
         * order as provided in the 'request' param. Keep this in mind if you
         * requested the last segment of the file, size of which may vary.
         *
         * @param request Specifies file and its segments to buffer.
         * @param result The array to buffer the file contents in.
         * @param size Size of the char array.
         */
        void get_file(FileRequest request, char *result, std::size_t size);

        /**
         * Stores the file contents in the physical file on disc.
         *
         * @param resource File to store on the disc.
         * The data will not be interpreted, so make sure it's complete and ready to
         * store.
         */
        void store_resource(CompleteResource &resource);

    private:
        // TODO: synchronizing access to the files.
    };
} // namespace simpleP2P

#endif // SIMPLE_P2P_FILE_MANAGER_H
