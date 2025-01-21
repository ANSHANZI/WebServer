#include"epoll.h"
#include<unistd.h>
#include"util.h"
#include"cstring"
#include"Channel.h"
#include <unistd.h>
#include <string.h>
#define MAX_EVENTS 1024

Epoll::Epoll(){
    epfd = epoll_create1(0);  //epoll 的文件描述符
    errif(epfd == -1, "epoll create error");
    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(*events) * MAX_EVENTS);
}


Epoll::~Epoll(){
     if(epfd != -1){
        close(epfd);
        epfd = -1;
    }
    delete [] events;
}

// void Epoll::addFd(int fd, uint32_t op){
//     struct epoll_event ev;
//     bzero(&ev, sizeof(ev));
//     ev.data.fd = fd;
//     ev.events = op;
//     errif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add event error");
// }

//返回已经就绪的epoll事件
std::vector<Channel*> Epoll::poll(int timeout){
    std::vector<Channel*> activeChannels;
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    errif(nfds == -1, "epoll wait error");
    for(int i = 0; i < nfds; ++i){
        Channel *ch = (Channel*)events[i].data.ptr;
        ch->setRevents(events[i].events);
        activeChannels.push_back(ch);
    }
    return activeChannels;
}

void Epoll::updateChannel(Channel* _channel){
    int fd=_channel->getFd();
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = _channel;
    ev.events = _channel->getEvents();   //拿到Channel希望监听的事件
    if(!_channel->getInEpoll()){
        //printf("add fd %d to epoll\n", fd);
        errif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");//添加Channel中的fd到epoll
        _channel->setInEpoll();
    } else{
        errif(epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll modify error");//已存在，则修改
    }
}   