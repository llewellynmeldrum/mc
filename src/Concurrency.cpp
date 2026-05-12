#include "Concurrency.hpp"
ThreadPool::~ThreadPool(){
    for (auto& jt: threads){
        if (jt.joinable()){
            jt.join();
        }else{
            std::println(stderr, "Unable to join thread");
            std::exit(-1);
        }
    }
}
