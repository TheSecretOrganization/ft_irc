#include "Server.hpp"

#include <cstdio>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

Server::Server(int port, const std::string& password)
	: fd(-1), port(port), password(password) {}

void Server::start() {
	fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
	if (fd == -1) {
		perror("socket");
		return;
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(fd, (sockaddr*)&addr, sizeof(addr))) {
		perror("bind");
		return;
	}

	if (listen(fd, 5) == -1) {
		perror("listen");
		return;
	}

	observer.subscribe(fd, *this);
	std::cout << "listening on port " << port << std::endl;

	while (true) {
		observer.wait();
	}
}

void Server::shut() {
	// TODO: delete socket, close connections
}

void Server::onPoll() {
	int clientFd = accept(fd, NULL, 0);
	if (clientFd == -1) {
		perror("accept");
		return;
	}

	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1) {
		perror("fcntl");
		return;
	}

	Client* client = new Client(clientFd);
	clients.push_back(client);
	observer.subscribe(clientFd, *client);
}
