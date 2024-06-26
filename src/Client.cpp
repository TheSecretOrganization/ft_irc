#include "Client.hpp"
#include "ClientSocket.hpp"

#include <exception>
#include <iostream>
#include <string>

Client::Client(int fd)
	: socket(fd), realname(""), username(""), nickname(""), hostname(""),
	  servername(""), status(UNKNOWN) {
	std::cout << "new client " << fd << std::endl;
	nickname = "";
}

Client::~Client() {}

ClientSocket& Client::getSocket() { return socket; }

std::string Client::getClientnickName(void) { return nickname; }

void Client::sendMessage(const std::string& type, const std::string& message,
						 const std::string& arg) const {
	std::string packet = (!arg.empty())
							 ? nickname + "!" + username + "@" + hostname +
								   " " + type + " " + arg + " :" + message
							 : nickname + "!" + username + "@" + hostname +
								   " " + type + " :" + message;
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

std::string Client::getHostname() const { return hostname; }

void Client::setHostname(const std::string& newHostname) {
	hostname = newHostname;
}

std::string Client::getServername() const { return servername; }

void Client::setServername(const std::string& newServername) {
	servername = newServername;
}

int Client::getStatus() const { return status; }

void Client::setStatus(int newStatus) { status = newStatus; }
