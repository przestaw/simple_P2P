
#include <GeneralTypes.h>
#include <iostream>
#include <logging_module.h>
#include <thread>
#include <random>

using namespace simpleP2P;

void test_worker(Int16 number){
    std::random_device random_device; //system specific random source

    for(Uint16 iter = 0; iter < 10; ++iter){
        Logging_Module::add_log_line("Message form " + std::to_string(number) + " number " + std::to_string(iter));
        std::this_thread::sleep_for(std::chrono::milliseconds(50 + random_device() % 350));
    }
}

int main() {
    std::thread basic[4];

    basic[3] = std::thread(&Logging_Module::logging_thread, std::ref(std::cout));

    for(Uint16 iter = 0; iter < 3; ++iter){
        basic[iter] = std::thread(test_worker, iter);
    }

    for(auto & iter : basic){
        iter.join();
    }
}
