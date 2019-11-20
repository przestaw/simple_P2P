//
// Created by przemek on 20.11.2019.
//

#ifndef SIMPLE_P2P_LOGGING_MODULE_H
#define SIMPLE_P2P_LOGGING_MODULE_H

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace simpleP2P {
    /**
     * @class Providing logging support based on text logs
     */
    class Logging_Module{
    public:
        /**
         * Main thread for the logging module
         * @param output Output stream for logging
         */
        static void logging_thread(std::ostream &output = std::cerr);
        /**
         * Synchronised method for logging output
         * @param line
         */
        static void add_log_line(std::string line);
    private:
        static std::queue<std::string> logging_queue;         //!< Queue of dispatched information from running program
        static std::mutex queue_mutex;                       //!< Mutex to secure exclusive access to the logging_queue
        static std::condition_variable queue_cond;
    };

}

#endif //SIMPLE_P2P_LOGGING_MODULE_H
