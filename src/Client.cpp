#include "Client.hpp"
#include "ClientSocket.hpp"

#include <exception>
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
	try {
		socket.sendPacket(packet);
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
