#pragma once
#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include<future>

class ThreadPool
{
private:
    std::vector<std::thread> threads;//线程池
    std::queue<std::function<void()>> tasks;
    std::mutex task_mutex;//任务队列锁
    std::condition_variable condition;//条件变量
    bool stop;//线程池停止标志
public:
    ThreadPool(int size=10);
    ~ThreadPool();

    //void add(std::function<void()> task);
    template<class F, class... Args>
    auto add(F&& f, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type>;

};



template<class F, class... Args>  //变长参数包 Args...
auto ThreadPool::add(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
    // -> std::future<typename std::result_of<F(Args...)>::type>  返回值类型
    using return_type = typename std::result_of<F(Args...)>::type; //
    auto task = std::make_shared< std::packaged_task<return_type()> >(  //
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)  //完美转发参数
        );  
        
    std::future<return_type> res = task->get_future();  // 使用期约
    {   //队列锁作用域
        std::unique_lock<std::mutex> lock(task_mutex);   //加锁
        if(stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");
        tasks.emplace([task](){ (*task)(); });  //将任务添加到任务队列
    }
    condition.notify_one();    //通知一次条件变量
    return res;     //返回一个期约
}




 