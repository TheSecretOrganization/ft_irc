#include "Client.hpp"
#include "Channel.hpp"
#include "ClientSocket.hpp"
#include "Server.hpp"

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

Client::Client(int fd)
	: socket(fd), realname(""), username(""), nickname(""), hostname(""),
	  servername(""), status(UNKNOWN), away(false), invisible(false) {
	std::cout << "new client " << fd << std::endl;
}

Client::~Client() {}

ClientSocket& Client::getSocket() { return socket; }

const std::string& Client::getClientnickName(void) { return nickname; }

void Client::sendMessage(const std::string& prefix, const std::string& command,
						 const std::string& parameters,
						 const std::string& trailing) const {
	std::string packet = prefix + " " + command;

	if (!parameters.empty())
		packet += " " + parameters;

	if (!trailing.empty())
		packet += (trailing[0] == ':' ? " " : " :") + trailing;

	try {
		socket.sendPacket(packet);
	} catch (const ClientSocket::SendException& e) {
		std::cerr << e.what() << std::endl;
	}
}

void Client::sendError(const std::string& command,
					   const std::string& parameters,
					   const std::string& trailing) const {
	try {
		sendMessage(Server::getInstance().getPrefix(), command, parameters,
					trailing);
	} catch (const ClientSocket::SendException& e) {
		std::cerr << e.what() << std::endl;
	}
}

std::string Client::getPrefix() const {
	return nickname + "!" + username + "@" + hostname;
}

std::string Client::getModes() const {
	std::string modes = "";

	if (away)
		modes += "a";

	return modes.empty() ? "" : "+" + modes;
}

std::vector<Channel*> Client::getJoinedChannels() const {
	const std::vector<Channel*>& channels = Server::getInstance().getChannels();
	std::vector<Channel*> userChan;

	for (size_t i = 0; i < channels.size(); ++i) {
		if (channels[i]->isUserOnChannel(const_cast<Client*>(this)))
			userChan.push_back(channels[i]);
	}

	return userChan;
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

bool Client::isAway() const { return away; }

void Client::setAway(bool newAway) { away = newAway; }

bool Client::isInvisible() const { return invisible; }

void Client::setInvisible(bool newInvisible) { invisible = newInvisible; }
