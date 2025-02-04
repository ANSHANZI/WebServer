#include"Channel.h"
#include "epoll.h"
#include"EventLoop.h"
#include<iostream>
Channel::Channel(Eventloop*_loop, int _fd) : loop(_loop), fd(_fd), events(0), revents(0), inEpoll(false){

}

Channel::~Channel(){

}

//设置为读事件
void Channel:: enableReading(){
    events = EPOLLIN | EPOLLET;
    loop->updateChannel(this);
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
    //printf("setInEpoll\n");
}
    // void setEvents(uint32_t);
void  Channel:: setRevents(uint32_t  _ev){
    revents= _ev;
}


void  Channel::handleEvent(){
    callback();
}

void Channel::setCallback(std::function<void()> _cb){
    callback = _cb;
}