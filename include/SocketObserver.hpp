#pragma once

#include "SocketListener.hpp"

class SocketObserver {

  private:
	int fd;

  public:
	SocketObserver();

	void subscribe(int fd, SocketListener& observer);
	void wait();
	void unsubscribe(int fd);
};
