#include "SocketObserver.hpp"
#include "Server.hpp"

#include <cstddef>
#include <cstdio>
#include <sys/epoll.h>
#include <unistd.h>

#define TIMEOUT 100

SocketObserver::SocketObserver() {
	fd = epoll_create1(0);
	if (fd == -1)
		throw EpollCreateException();
}

SocketObserver::~SocketObserver() {
	if (fd >= 0)
		close(fd);
}

void SocketObserver::subscribe(int fd, Socket& observer) {
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = fd;
	ev.data.ptr = &observer;

	if (epoll_ctl(this->fd, EPOLL_CTL_ADD, fd, &ev) == -1)
		throw EpollCtlAddException();
}

void SocketObserver::poll() {
	struct epoll_event events[MAX_POLL];
	int nfds = epoll_wait(fd, events, MAX_POLL, TIMEOUT);
	if (nfds == -1) {
		if (Server::getInstance().getRun() == false)
			return;
		throw EpollWaitException();
	}

	for (int i = 0; i < nfds; i++)
		((Socket*)events[i].data.ptr)->onPoll();
}

void SocketObserver::unsubscribe(int fd) {
	if (epoll_ctl(this->fd, EPOLL_CTL_DEL, fd, NULL))
		throw EpollCtlDelException();
}

const char* SocketObserver::EpollCreateException::what() const throw() {
	return "cannot create epoll fd";
}

const char* SocketObserver::EpollWaitException::what() const throw() {
	return "epoll_wait exception";
}

const char* SocketObserver::EpollCtlAddException::what() const throw() {
	return "cannot create new subscriber";
}

const char* SocketObserver::EpollCtlDelException::what() const throw() {
	return "cannot delete subscriber";
}
