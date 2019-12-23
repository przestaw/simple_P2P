//
// Created by przemek on 20.11.2019.
//

#ifndef SIMPLE_P2P_LOGGING_MODULE_H
#define SIMPLE_P2P_LOGGING_MODULE_H

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

namespace simpleP2P {
    /**
     * class Providing logging support based on text logs
     */
    class Logging_Module{
    public:
        /**
         * Constructor for the logging thread
         * @param output_c Output stream for the logs
         * @note if output stream is a file you must explicitly close it
         */
        Logging_Module(std::ostream &output_c = std::cerr);

        /**
         * Init methods run worker in thread and returns it
         * @return logging thread
         */
        std::thread init();

        /**
         * Synchronised method for logging output
         * @param line
         */
        void add_log_line(std::string line, const std::time_t time);
    private:
        /**
         * Main loop of the logging module
         */
        void worker();

        std::ostream &output;                         //!< Output stream for logs
        std::queue<std::string> logging_queue;        //!< Queue of dispatched information from running program
        std::mutex queue_mutex;                       //!< Mutex to secure exclusive access to the logging_queue
        std::condition_variable queue_cond;           //!< Cond variable for main loop notification
    };

}

#endif //SIMPLE_P2P_LOGGING_MODULE_H
