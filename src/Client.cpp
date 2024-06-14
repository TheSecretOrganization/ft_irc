#include "Client.hpp"
#include "ClientSocket.hpp"

#include <iostream>

Client::Client(int fd): socket(fd) {
	std::cout << "new client " << fd << std::endl;
}

Client::~Client() {}

ClientSocket& Client::getSocket() {
	return socket;
}
