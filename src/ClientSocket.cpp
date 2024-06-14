#include "ClientSocket.hpp"

#include <cstdio>
#include <iostream>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>

#define SIZE 100

ClientSocket::ClientSocket(int fd): fd(fd) {}

ClientSocket::~ClientSocket() {
    close(fd);
}

int ClientSocket::getFd() const {
    return fd;
}

void ClientSocket::onPoll() {
	char buff[SIZE];
	std::string content;

	bzero(buff, SIZE);
	while (content[content.size() - 1] != '\n') {
		ssize_t size = recv(fd, buff, SIZE, 0);
		if (size == -1) {
			perror("recv");
			return;
		}

		content.append(buff);
		bzero(buff, SIZE);
	}
	content.erase(content.size() - 1, content.size());
	std::cout << "from " << fd << ": " << content << std::endl;
}
