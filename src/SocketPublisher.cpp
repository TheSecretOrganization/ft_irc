#include "SocketPublisher.hpp"

#include <cstdio>
#include <sys/epoll.h>

SocketPublisher::SocketPublisher() {
	fd = epoll_create1(0);
	if (fd == -1) {
		perror("epoll_create");
		return ;
	}
}

void SocketPublisher::subscribe(int fd, SocketSubscriber &observer) {
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = fd;
	ev.data.ptr = &observer;

	if (epoll_ctl(this->fd, EPOLL_CTL_ADD, fd, &ev) == -1) {
		perror("epoll_ctl");
		return ;
	}
}

void SocketPublisher::wait() {
	struct epoll_event events[10];
	int nfds = epoll_wait(fd, events, 10, -1);
	if (nfds == -1) {
		perror("epoll_wait");
		return ;
	}

	for (int i = 0; i < nfds; i++)
		((SocketSubscriber *) events[i].data.ptr)->onPoll();
}

void SocketPublisher::unsubscribe(int fd) {
	//TODO: remove fd from epoll
}
