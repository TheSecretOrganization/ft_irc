#include "SocketObserver.hpp"

#include <cstdio>
#include <sys/epoll.h>

SocketObserver::SocketObserver() {
	fd = epoll_create1(0);
	if (fd == -1) {
		perror("epoll_create");
		return;
	}
}

void SocketObserver::subscribe(int fd, SocketListener& observer) {
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = fd;
	ev.data.ptr = &observer;

	if (epoll_ctl(this->fd, EPOLL_CTL_ADD, fd, &ev) == -1) {
		perror("epoll_ctl");
		return;
	}
}

void SocketObserver::poll() {
	struct epoll_event events[10];
	int nfds = epoll_wait(fd, events, 10, -1);
	if (nfds == -1) {
		perror("epoll_wait");
		return;
	}

	for (int i = 0; i < nfds; i++)
		((SocketListener*)events[i].data.ptr)->onPoll();
}

void SocketObserver::unsubscribe(int fd) {
	// TODO: remove fd from epoll
	(void)fd;
}
