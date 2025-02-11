#include "include/Channel.h"
#include "include/EventLoop.h"
#include "include/epoll.h"
#include <iostream>
#include <unistd.h>
Channel::Channel(Eventloop *_loop, int _fd)
    : loop(_loop), fd(_fd), events(0), ready(0), inEpoll(false) {}

Channel::~Channel() {
  if (fd != -1) {
    close(fd);
    fd = -1;
  }
}

void Channel::handleEvent() {
  // 检查是否为读事件（EPOLLIN 或 EPOLLPRI）
  if (ready & (EPOLLIN | EPOLLPRI)) {
    readCallback(); // 直接在当前线程处理读事件
  }
  // 检查是否为写事件（EPOLLOUT）
  if (ready & (EPOLLOUT)) {
    writeCallback(); // 直接在当前线程处理写事件
  }
}

//设置为读事件
void Channel::enableReading() {
  events = EPOLLIN | EPOLLET;
  loop->updateChannel(this);
}

int Channel::getFd() { return fd; }

uint32_t Channel::getEvents() { return events; }
uint32_t Channel::getReady() { return ready; }

bool Channel::getInEpoll() { return inEpoll; }

void Channel::setInEpoll(bool _in) {
  inEpoll = true;
  // printf("setInEpoll\n");
}
// void setEvents(uint32_t);
void Channel::setready(uint32_t _ev) { ready = _ev; }

void Channel::setreadCallback(std::function<void()> _cb) { readCallback = _cb; }

void Channel::useET() {
  events |= EPOLLET;
  loop->updateChannel(this);
}