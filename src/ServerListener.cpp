#include "ServerListener.hpp"
#include "Client.hpp"
#include "Server.hpp"

#include <cstdio>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

ServerListener::ServerListener() : fd(-1), port(-1) {}

ServerListener::~ServerListener() { close(fd); }

void ServerListener::init(int port) {
	this->port = port;
	fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
	if (fd == -1) {
		perror("listener");
		return;
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(this->port);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(fd, (sockaddr*)&addr, sizeof(addr))) {
		perror("bind");
		return;
	}

	if (listen(fd, 5) == -1) {
		perror("listen");
		return;
	}

	std::cout << "listening on port " << this->port << std::endl;
}

void ServerListener::onPoll() {
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
	Server::getInstance().addClient(client);
}

int ServerListener::getFd() const { return fd; }
