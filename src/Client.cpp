#include "Client.hpp"

#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>

#define SIZE 100

Client::Client(int fd) : fd(fd) {
	std::cout << "new client " << fd << std::endl;
}

void Client::onPoll() {
	char buff[SIZE];
	std::string content;
	
	while (content[content.size() - 1] != '\n') {
		ssize_t size = recv(fd, buff, SIZE, 0);
		if (size == -1) {
			perror("recv");
			return ;
		}

		content.append(buff);
		bzero(buff, SIZE);
	}
	content.erase(content.size() - 1, content.size());
	std::cout << "from " << fd << ": " << content << std::endl;
}
