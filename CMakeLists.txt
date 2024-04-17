cmake_minimum_required(VERSION 3.23)
project(ThreadPools)

set(CMAKE_CXX_STANDARD 17  -std=c++17)

add_executable(ThreadPools main.cpp cancel.h cancel.cpp executor.h thread_pool.h thread_pool.cpp io_util.h io_util.cpp functional.h visibility.h macros.h main.cpp status.h)
