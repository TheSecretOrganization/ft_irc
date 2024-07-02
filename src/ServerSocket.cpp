#include "ServerSocket.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Socket.hpp"

#include <cstdio>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

ServerSocket::ServerSocket() : Socket(-1), port(-1) {}

ServerSocket::~ServerSocket() {}

void ServerSocket::init(int port) {
	this->port = port;
	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd == -1)
		throw ServerSocket();

	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
		throw FcntlException();

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(this->port);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(fd, (sockaddr*)&addr, sizeof(addr)))
		throw BindException();

	if (listen(fd, 5) == -1)
		throw ListenException();

	std::cout << "listening on port " << this->port << std::endl;
}

void ServerSocket::onPoll() {
	int clientFd = accept(fd, NULL, 0);
	if (clientFd == -1)
		throw AcceptException();

	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
		throw FcntlException();

	Client* client = new Client(clientFd);
	Server::getInstance().addClient(client);
}

const char* ServerSocket::SocketException::what() const throw() {
	return "cannot create server's socket";
}

const char* ServerSocket::BindException::what() const throw() {
	return "cannot bind server's socket to local address";
}

const char* ServerSocket::ListenException::what() const throw() {
	return "cannot listen on local port";
}

const char* ServerSocket::AcceptException::what() const throw() {
	return "cannot accpet new connection";
}

const char* ServerSocket::FcntlException::what() const throw() {
	return "fcntl exception";
}
