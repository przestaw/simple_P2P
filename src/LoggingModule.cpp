//
// Created by przemek on 20.11.2019.
//

#include "LoggingModule.h"

namespace simpleP2P {
LoggingModule::LoggingModule(std::ostream &output_c) : output(output_c) {}

std::thread LoggingModule::init() {
  return std::thread([&] { worker(); });
}

void LoggingModule::worker() {
  std::unique_lock<std::mutex> uniqueLock(queue_mutex);
  for (;;) {
    queue_cond.wait(uniqueLock);
    while (logging_queue.size()) {
      output << logging_queue.front() << '\n';
      logging_queue.pop();
    }
  }
}

void LoggingModule::add_log_line(std::string line, const std::time_t &time) {
  std::lock_guard<std::mutex> uniqueLock(queue_mutex);
  auto time_str = std::string(std::ctime(&time));
  logging_queue.push("[" +
      time_str.substr(0, time_str.length() - 1) +
      "] " + line);
  queue_cond.notify_one();
}
} // namespace simpleP2P
