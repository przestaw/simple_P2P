//
// Created by przemek on 20.11.2019.
//

#include "logging_module.h"

namespace simpleP2P{
    Logging_Module::Logging_Module(std::ostream &output_c) : output(output_c) {}

    std::thread Logging_Module::init() {
        return std::thread([=] { worker(); });
    }

    void Logging_Module::worker() {
        std::unique_lock<std::mutex> uniqueLock(queue_mutex);
        for(;;){
            queue_cond.wait(uniqueLock);
            while(logging_queue.size()){
                output << logging_queue.back() << '\n';
                logging_queue.pop();
            }
        }
    }

    void Logging_Module::add_log_line(std::string line, const std::time_t time) {
        std::lock_guard<std::mutex> uniqueLock(queue_mutex);
        logging_queue.push("[" + std::string(std::ctime(&time)) + "]" + line);
        queue_cond.notify_one();
    }
}