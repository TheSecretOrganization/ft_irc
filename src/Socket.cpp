#include "Socket.hpp"

#include <unistd.h>

Socket::Socket(int fd) : fd(fd) {}

Socket::~Socket() {
	if (fd >= 0)
		close(fd);
}

int Socket::getFd() const { return fd; }
