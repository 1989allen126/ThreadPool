cmake_minimum_required(VERSION 3.7)
project(ThreadPool)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -Wno-unused-variable -pthread")

# 包含目录
include_directories(./Source)

set(SOURCE_FILES
        Main.cpp Source/CThread.cpp
        Source/CThread.h
        Source/CCondition.cpp
        Source/CCondition.h
        Source/CThreadMutex.cpp
        Source/CThreadMutex.h
        Source/CJob.h
        Source/CJob.cpp
        Source/CWorkerThread.h
        Source/CWorkerThread.cpp
        MyJob.cpp MyJob.h
        Source/CWorkerThread.cpp
        Source/CWorkerThread.h
        Source/CThreadPool.cpp
        Source/CThreadPool.h
        Source/CThreadManage.h
        Source/CThreadManage.cpp)

add_executable(ThreadPool ${SOURCE_FILES})

target_link_libraries(ThreadPool pthread)