//
// Created by WYZ on 2024/4/8.
//
#include <iostream>
#include "thread_pool.h"
#include "executor.h"
#include "io_util.h"
using namespace arrow;

//std::mutex consoleMutex;
//void task(int id) {
//    // Lock the console before printing
//    std::lock_guard<std::mutex> lock(consoleMutex);
//    // Get the ID of the current thread
//    std::thread::id threadId = std::this_thread::get_id();
//
//    std::cout << "Task " << id << " started by thread " << threadId << std::endl;
//    std::this_thread::sleep_for(std::chrono::seconds(2));
//    std::cout << "Task " << id << " completed by thread " << threadId << std::endl;
//}
//
//int main() {
//    SetEnvVar("OMP_NUM_THREADS", "4");
//
//    // Create a thread pool with 4 worker threads
//    auto threadPool = GetCpuThreadPool();
//    if (!threadPool) {
//        std::cerr << "Failed to create thread pool" << std::endl;
//        return 1;
//    }
//
//    // Submit tasks to the thread pool
//    for (int i = 1; i <= 8; ++i) {
//        threadPool->Spawn([i]() { task(i); });
//    }
//
//    // Wait for all tasks to complete
//    threadPool->WaitForIdle();
//
//    // Shutdown the thread pool
//    threadPool->Shutdown();
//}


template <typename T>
static T add(T x, T y) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return x + y;
}
template <typename T>
static T sub(T x, T y) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return x - y;
}
template <typename T>
static T mul(T x, T y) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return x * y;
}

void status_callback(const Status& status) {
    std::cout << "status_callback: " << status.ToString() << std::endl;
}

int main() {
    arrow::StopSource stop_source;
    arrow::StopToken stop_token = stop_source.token();
    auto threadPool = GetCpuThreadPool();

    int a = 10;
    int b = 20;

    std::vector<std::future<int>> futures;
    // Submit multiple tasks
    auto add_fut = threadPool->Submit(arrow::TaskHints{}, stop_token, status_callback, [a, b]() { return add(a, b); });
    auto sub_fut = threadPool->Submit([a, b]() { return sub(a, b); });
    auto mul_fut = threadPool->Submit(stop_token, [a, b]() { return mul(a, b); });
    futures.push_back(std::move(add_fut));
    futures.push_back(std::move(sub_fut));
    futures.push_back(std::move(mul_fut));
    stop_source.RequestStop();

    // Retrieve the results
    for (auto& future : futures) {
        try {
            int result = future.get();
            std::cout << "Result: " << result << std::endl;
        } catch (const std::exception& e) { /* will throw error. */
            std::cerr << "Exception occurred: " << e.what() << std::endl;
        }
    }


    return 0;
}



//template <typename T>
//static T add(T x, T y) {
//    return x + y;
//}
//template <typename T>
//static T sub(T x, T y) {
//    return x - y;
//}
//template <typename T>
//static T mul(T x, T y) {
//    return x * y;
//}
//
//int sum(int a, int b) { return add(a, b) + sub(a, b) + mul(a, b); }
//
//int main() {
//    auto threadPool = GetCpuThreadPool();
//
//    int a = 10;
//    int b = 20;
//
//    std::vector<std::future<int>> futures;
//    // Submit multiple tasks
//    auto add_fut = threadPool->Submit([a, b]() { return add(a, b); });
//    auto sub_fut = threadPool->Submit([a, b]() { return sub(a, b); });
//    auto mul_fut = threadPool->Submit([a, b]() { return mul(a, b); });
//    auto params_fut = threadPool->Submit(sum, a, b);
//    futures.push_back(std::move(add_fut));
//    futures.push_back(std::move(sub_fut));
//    futures.push_back(std::move(mul_fut));
//    futures.push_back(std::move(params_fut));
//
//    // Retrieve the results
//    for (auto& future : futures) {
//        try {
//            int result = future.get();
//            std::cout << "Result: " << result << std::endl;
//        } catch (const std::exception& e) {
//            std::cerr << "Exception occurred: " << e.what() << std::endl;
//        }
//    }
//
//    return 0;
//}