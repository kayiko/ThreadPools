//
// Created by WYZ on 2024/4/8.
//
#include <iostream>
#include "thread_pool.h"
#include "executor.h"
using namespace arrow;

int main(){
    auto threadPool = GetCpuThreadPool();
    if (!threadPool) {
        std::cerr << "Failed to create thread pool" << std::endl;
        return 1;
    }
    // Submit tasks to the thread pool
    threadPool->Spawn([]() { std::cout << "hello world!!!!!" << std::endl; });

    // Wait for all tasks to complete
    threadPool->WaitForIdle();

    // Shutdown the thread pool
    threadPool->Shutdown();
}