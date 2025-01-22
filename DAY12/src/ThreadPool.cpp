#include "include/ThreadPool.h"

ThreadPool::ThreadPool(int size) : stop(false) {
  for (int i = 0; i < size; i++) { // std::thread([this]() { /* 执行的代码 */
                                   // });
    threads.push_back(std::thread([this]() { //创建线程
      while (true) { //每个线程都会执行这个循环
        std::function<void()> task;
        //{}作用域内对std::mutex加锁，出了作用域会自动解锁，不需要调用unlock()
        {
          std::unique_lock<std::mutex> lock(task_mutex);
          condition.wait(
              lock, [this]() { //等待条件变量，条件为任务队列不为空或线程池停止
                return stop || !tasks.empty();
              });
          if (stop && tasks.empty())
            return; //任务队列为空并且线程池停止，退出线程
          task = tasks.front(); //从任务队列头取出一个任务
          tasks.pop();
        }
        task(); //执行任务
      }
    }));
  }
}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(task_mutex);
    stop = true; //线程池停止
  }
  condition.notify_all();           //通知所有条件变量
  for (std::thread &th : threads) { //等待所有线程结束
    if (th.joinable())
      th.join(); //等待所有线程完成并清理资源
  }
}

// void ThreadPool::add(std::function<void()> func){
//      //在这个{}作用域内对std::mutex加锁，出了作用域会自动解锁，不需要调用unlock()
//     {
//         std::unique_lock<std::mutex> lock(task_mutex);
//         if(stop)
//             throw std::runtime_error("ThreadPool already stop, can't add task
//             any more");
//         tasks.emplace(func);
//     }
//     condition.notify_one();    //通知一次条件变量
// }