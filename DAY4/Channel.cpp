#include"Channel.h"
#include "epoll.h"
Channel::Channel(Epoll *_ep, int _fd):ep(_ep),fd( _fd),events(0), revents(0), inEpoll(false){

}
Channel::~Channel(){

}

//设置为读事件
void Channel:: enableReading(){
    events = EPOLLIN | EPOLLET;
    ep->updateChannel(this);
}

int Channel::getFd(){
    return fd;
}


uint32_t Channel::getEvents(){
    return events;
}
uint32_t Channel::getRevents(){
    return revents;
}

bool Channel::getInEpoll(){
    return inEpoll;
}

void Channel::setInEpoll(){
    inEpoll = true;  
}
    // void setEvents(uint32_t);
void  Channel:: setRevents(uint32_t  _ev){
    revents= _ev;
}

