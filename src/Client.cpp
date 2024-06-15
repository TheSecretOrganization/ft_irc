#include "Client.hpp"
#include "ClientSocket.hpp"

#include <iostream>
#include <string>

Client::Client(int fd) : socket(fd) {
	std::cout << "new client " << fd << std::endl;
}

Client::~Client() {}

ClientSocket& Client::getSocket() { return socket; }

void Client::sendMessage(std::string type, std::string message) const {
	(void) type;
	std::string packet = message;
	socket.sendPacket(packet);
}
