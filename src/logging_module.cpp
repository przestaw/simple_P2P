//
// Created by przemek on 20.11.2019.
//

#include "logging_module.h"

namespace simpleP2P{
    std::queue<std::string> Logging_Module::logging_queue;
    std::mutex Logging_Module::queue_mutex;
    std::condition_variable Logging_Module::queue_cond;

    void Logging_Module::logging_thread(std::ostream &output) {
        std::unique_lock<std::mutex> uniqueLock(queue_mutex);
        for(;;){
            queue_cond.wait(uniqueLock);
            while(logging_queue.size()){
                output << logging_queue.back() << '\n';
                logging_queue.pop();
            }
        }
    }

    void Logging_Module::add_log_line(std::string line) {
        std::lock_guard<std::mutex> uniqueLock(queue_mutex);
        logging_queue.push(line);
        queue_cond.notify_one();
    }
}
