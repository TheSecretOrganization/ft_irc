#include "Client.hpp"
#include "ClientSocket.hpp"

#include <exception>
#include <iostream>
#include <string>

Client::Client(int fd) : socket(fd), realname(""), username(""), nickname("") {
	std::cout << "new client " << fd << std::endl;
}

Client::~Client() {}

ClientSocket& Client::getSocket() { return socket; }

void Client::sendMessage(std::string type, std::string message) const {
	std::string packet = type + " " + message;
	try {
		socket.sendPacket(packet);
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

std::string Client::getNickname() const { return nickname; }

void Client::setNickname(const std::string& newNickname) {
	nickname = newNickname;
}

std::string Client::getUsername() const { return username; }

void Client::setUsername(const std::string& newUsername) {
	username = newUsername;
}

std::string Client::getRealname() const { return realname; }

void Client::setRealname(const std::string& newRealname) {
	realname = newRealname;
}
