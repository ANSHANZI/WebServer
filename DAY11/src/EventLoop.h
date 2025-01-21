#pragma once
#include <functional>
class Epoll;
class Channel;
class ThreadPool;
class Eventloop{
private:
    Epoll *ep;
    bool quit;
public:
    Eventloop();
    ~Eventloop();

    void loop();
    void updateChannel(Channel*);
    void addThread(std::function<void()>);
};