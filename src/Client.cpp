#include "Client.hpp"
#include "ClientSocket.hpp"

#include <iostream>
#include <string>

Client::Client(int fd) : socket(fd) {
	std::cout << "new client " << fd << std::endl;
	nickname = "";
}

Client::~Client() {}

ClientSocket& Client::getSocket() { return socket; }

std::string Client::getClientnickName(void) { return nickname; }

void Client::sendMessage(std::string type, std::string message) const {
	std::string packet = type + " " + message;
	socket.sendPacket(packet);
}
