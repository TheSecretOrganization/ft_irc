#include "Server.hpp"
#include "Client.hpp"

#include <cstdio>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

Server::Server() {}

Server::~Server() {
	for (std::vector<Client*>::iterator it = clients.begin();
		 it != clients.end(); ++it) {
		observer.unsubscribe((*it)->getFd());
		delete *it;
	}

	observer.unsubscribe(fd);
	close(fd);
}

Server& Server::getInstance() {
	static Server instance;
	return instance;
}

void Server::start(int port, const std::string& password) {
	this->port = port;
	this->password = password;
	run = true;
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

	while (run) {
		observer.poll();
	}
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

void Server::setRun(bool newState) {
	run = newState;
}
