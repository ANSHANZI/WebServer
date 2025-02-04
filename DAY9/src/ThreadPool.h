#pragma once
#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
class ThreadPool
{
private:
    std::vector<std::thread> threads;//线程池
    std::queue<std::function<void()>> tasks;
    std::mutex task_mutex;//任务队列锁
    std::condition_variable condition;//条件变量
    bool stop;//线程池停止标志
public:
    ThreadPool(int size);
    ~ThreadPool();

    void add(std::function<void()> task);
};


