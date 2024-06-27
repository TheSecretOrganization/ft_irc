#include "Client.hpp"
#include "ClientSocket.hpp"

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

const std::string& Client::getClientnickName(void) { return nickname; }

void Client::sendMessage(const std::string& type, const std::string& message,
						 const std::string& arg) const {
	std::string packet = (!arg.empty())
							 ? nickname + "!" + username + "@" + hostname +
								   " " + type + " " + arg + " :" + message
							 : nickname + "!" + username + "@" + hostname +
								   " " + type + " :" + message;
	try {
		socket.sendPacket(packet);
	} catch (const ClientSocket::SendException& e) {
		std::cerr << e.what() << std::endl;
	}
}

void Client::sendError(const std::string& code, const std::string& message,
					   const std::string& arg) const {
	try {
		if (!message.empty())
			sendMessage(code, arg + " :" + message);
		else
			sendMessage(code, ":" + message);
	} catch (const ClientSocket::SendException& e) {
		std::cerr << e.what() << std::endl;
	}
}

const std::string& Client::getNickname() const { return nickname; }

void Client::setNickname(const std::string& newNickname) {
	nickname = newNickname;
}

const std::string& Client::getUsername() const { return username; }

void Client::setUsername(const std::string& newUsername) {
	username = newUsername;
}

const std::string& Client::getRealname() const { return realname; }

void Client::setRealname(const std::string& newRealname) {
	realname = newRealname;
}

const std::string& Client::getHostname() const { return hostname; }

void Client::setHostname(const std::string& newHostname) {
	hostname = newHostname;
}

const std::string& Client::getServername() const { return servername; }

void Client::setServername(const std::string& newServername) {
	servername = newServername;
}

int Client::getStatus() const { return status; }

void Client::setStatus(int newStatus) { status = newStatus; }
