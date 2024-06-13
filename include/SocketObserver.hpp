#pragma once

#include "SocketListener.hpp"

class SocketObserver {

  private:
	int fd;

  public:
	SocketObserver();
	~SocketObserver();

	void subscribe(int fd, SocketListener& observer);
	void poll();
	void unsubscribe(int fd);
};
