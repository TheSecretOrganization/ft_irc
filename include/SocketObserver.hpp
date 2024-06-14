#pragma once

#include "Socket.hpp"

#define MAX_POLL 10

class SocketObserver {

  private:
	int fd;

  public:
	SocketObserver();
	~SocketObserver();

	void subscribe(int fd, Socket& observer);
	void poll();
	void unsubscribe(int fd);
};
