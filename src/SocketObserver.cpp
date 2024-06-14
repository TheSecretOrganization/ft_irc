#include "SocketObserver.hpp"

#include <cstddef>
#include <cstdio>
#include <sys/epoll.h>
#include <unistd.h>

#define TIMEOUT 100

SocketObserver::SocketObserver() {
	fd = epoll_create1(0);
	if (fd == -1) {
		perror("epoll_create");
		return;
	}
}

SocketObserver::~SocketObserver() { close(fd); }

void SocketObserver::subscribe(int fd, Listener& observer) {
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
	struct epoll_event events[MAX_POLL];
	int nfds = epoll_wait(fd, events, MAX_POLL, TIMEOUT);
	if (nfds == -1) {
		perror("epoll_wait");
		return;
	}

	for (int i = 0; i < nfds; i++)
		((Listener*)events[i].data.ptr)->onPoll();
}

void SocketObserver::unsubscribe(int fd) {
	if (epoll_ctl(this->fd, EPOLL_CTL_DEL, fd, NULL)) {
		perror("epoll_ctl");
		return;
	}
}
