# ThreadPools
This is a light-Thread Pools Project!

Env: c++17  GCC:x86_64-8.1.0-release-posix-sjlj

# Quick Star

### Example 1

```c++
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "src/cancel.h"
#include "src/io_util.h"
#include "src/macros.h"
#include "src/thread_pool.h"
using namespace arrow;

int main() {
  // 创建线程池
  auto threadPool = GetCpuThreadPool();
  if (!threadPool) {
    std::cerr << "Failed to create thread pool" << std::endl;
    return 1;
  }

  // 提交任务到线程池
  threadPool->Spawn([]() { std::cout << "hello world!" << std::endl; });

  // 等待线程执行完毕
  threadPool->WaitForIdle();

  // 关闭线程池回收资源
  threadPool->Shutdown();
}

```

## Example 2

```c++
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "src/cancel.h"
#include "src/io_util.h" 
#include "src/macros.h"
#include "src/thread_pool.h"
using namespace arrow;

std::mutex consoleMutex;
void task(int id) {

  std::lock_guard<std::mutex> lock(consoleMutex);

  std::thread::id threadId = std::this_thread::get_id();

  std::cout << "Task " << id << " started by thread " << threadId << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(2));
  std::cout << "Task " << id << " completed by thread " << threadId << std::endl;
}

int main() {
  SetEnvVar("OMP_NUM_THREADS", "4");
  auto threadPool = GetCpuThreadPool();
  
  if (!threadPool) {
    std::cerr << "Failed to create thread pool" << std::endl;
    return 1;
  }
  
  for (int i = 1; i <= 2; ++i) {
    threadPool->Spawn([i]() { task(i); });
  }

  threadPool->WaitForIdle();
  threadPool->Shutdown();
}

```

## Example 3

```c++
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "src/cancel.h"
#include "src/io_util.h"
#include "src/macros.h"
#include "src/thread_pool.h"
using namespace arrow;

template <typename T>
static T add(T x, T y) {
  return x + y;
}
template <typename T>
static T sub(T x, T y) {
  return x - y;
}
template <typename T>
static T mul(T x, T y) {
  return x * y;
}

int sum(int a, int b) { return add(a, b) + sub(a, b) + mul(a, b); }

int main() {
  auto threadPool = GetCpuThreadPool();

  int a = 10;
  int b = 20;

  std::vector<std::future<int>> futures;

  auto add_fut = threadPool->Submit([a, b]() { return add(a, b); });
  auto sub_fut = threadPool->Submit([a, b]() { return sub(a, b); });
  auto mul_fut = threadPool->Submit([a, b]() { return mul(a, b); });
  auto params_fut = threadPool->Submit(sum, a, b);
  futures.push_back(std::move(add_fut));
  futures.push_back(std::move(sub_fut));
  futures.push_back(std::move(mul_fut));
  futures.push_back(std::move(params_fut));

  for (auto& future : futures) {
    try {
      int result = future.get();
      std::cout << "Result: " << result << std::endl;
    } catch (const std::exception& e) {
      std::cerr << "Exception occurred: " << e.what() << std::endl;
    }
  }

  return 0;
}
```

## Example 4

```c++
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "src/cancel.h"
#include "src/io_util.h"
#include "src/macros.h"
#include "src/thread_pool.h"
using namespace arrow;

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

  auto add_fut = threadPool->Submit(arrow::TaskHints{}, stop_token, status_callback, [a, b]() { return add(a, b); });
  auto sub_fut = threadPool->Submit([a, b]() { return sub(a, b); });
  auto mul_fut = threadPool->Submit(stop_token, [a, b]() { return mul(a, b); });
  futures.push_back(std::move(add_fut));
  futures.push_back(std::move(sub_fut));
  futures.push_back(std::move(mul_fut));
  stop_source.RequestStop();

  for (auto& future : futures) {
    try {
      int result = future.get();
      std::cout << "Result: " << result << std::endl;
    } catch (const std::exception& e) { 
      std::cerr << "Exception occurred: " << e.what() << std::endl;
    }
  }

  return 0;
}

```

