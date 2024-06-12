#pragma once

#include "SocketSubscriber.hpp"

class SocketPublisher {

private:
	int fd;

public:
	SocketPublisher();

	void subscribe(int fd, SocketSubscriber &observer);
	void wait();
	void unsubscribe(int fd);

};
