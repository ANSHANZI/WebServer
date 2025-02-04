#pragma once
#include <sys/epoll.h>
#include <functional>
//Channel类可以将文件描述符与其对应的服务处理逻辑封装在一起，使得不同服务的处理更加清晰和易于管理。
//一个文件描述符封装成一个Channel类，一个Channel类自始至终只负责一个文件描述符，
//对不同的服务、不同的事件类型，都可以在类中进行不同的处理，而不是仅仅拿到一个int类型的文件描述符。
class Eventloop;
class Channel{
private:
    Eventloop *loop;
    int fd;//
    uint32_t events;//表示希望监听这个文件描述符的哪些事件
    uint32_t ready;//EPOLL 监听到的事件
    bool inEpoll;//表示当前Channel是否已经在epoll红黑树中
    bool useThreadPool;//是否使用线程池

    std::function<void()> readCallback;//读回调函数
    std::function<void()> writeCallback;//写回调函数

public:
    Channel(Eventloop *_loop, int _fd);
    ~Channel();

    void handleEvent();
    void enableReading();

    int getFd();
    uint32_t getEvents();
    uint32_t getReady();
    bool getInEpoll();

    void setInEpoll(bool _in = true);

    void useET();//设置为ET模式
    // void setEvents(uint32_t);
    void setready(uint32_t);
    void setreadCallback(std::function<void()>);
    void setUseThreadPool(bool use = true);
};