#pragma once

#include "Listener.hpp"

#define MAX_POLL 10

class SocketObserver {

  private:
	int fd;

  public:
	SocketObserver();
	~SocketObserver();

	void subscribe(int fd, Listener& observer);
	void poll();
	void unsubscribe(int fd);
};
