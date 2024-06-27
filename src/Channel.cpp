#include "Channel.hpp"
#include "Server.hpp"

#include <algorithm>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

void Channel::checkChannelSyntax(const std::string& channelName) {
	if (channelName[0] != '#') {
		throw Channel::InvalidChannelPrefixException();
	}

	if (channelName.find(" ") != channelName.npos) {
		throw Channel::ForbiddenChannelNameException(' ');
	}
	if (channelName.find(0x07) != channelName.npos) {
		throw Channel::ForbiddenChannelNameException(0x07);
	}
	if (channelName.find(",") != channelName.npos) {
		throw Channel::ForbiddenChannelNameException(',');
	}
}

Channel::Channel(Client* creator, const std::string& name) : name(name) {
	checkChannelSyntax(name);
	operators.push_back(creator);
	usersOnChannel.push_back(creator);
	topic = "";
	inviteOnly = 0;
	channelSize = DEF_CHAN_SIZE;
}

Channel::Channel(Client* creator, const std::string& name,
				 const std::string& password)
	: name(name), channelPassword(password) {
	checkChannelSyntax(name);
	operators.push_back(creator);
	usersOnChannel.push_back(creator);
	topic = "";
	inviteOnly = 0;
	channelSize = DEF_CHAN_SIZE;
}

Channel::~Channel() {}

void Channel::createChannel(Client* client, const std::string& name,
							const std::string& password) {
	Channel* newChannel = new Channel(client, name, password);
	Server::getInstance().addChannel(newChannel);

	std::cout << "Successfully created channel " << name << std::endl;
	std::cout << "operators: "
			  << (*newChannel->getOperators().begin())->getClientnickName()
			  << std::endl;
	std::cout << "users: "
			  << (*newChannel->getUsers().begin())->getClientnickName()
			  << std::endl;
}

std::vector<Client*>& Channel::getUsers(void) { return usersOnChannel; }

std::vector<Client*>& Channel::getOperators(void) { return operators; }

std::vector<Client*>& Channel::getInviteList(void) { return inviteList; }

const std::string& Channel::getChannelName(void) const { return name; }

bool Channel::isUserOnChannel(Client* client) {
	int clientFd = client->getSocket().getFd();

	for (std::vector<Client*>::iterator it = usersOnChannel.begin();
		 it != usersOnChannel.end(); ++it) {
		if ((*it)->getSocket().getFd() == clientFd)
			return 1;
	}
	return 0;
}

bool Channel::isUserOperator(Client* client) {
	if (!Channel::isUserOnChannel(client))
		return 0;

	int clientFd = client->getSocket().getFd();

	for (std::vector<Client*>::iterator it = operators.begin();
		 it != operators.end(); ++it) {
		if ((*it)->getSocket().getFd() == clientFd)
			return 1;
	}
	return 0;
}

bool Channel::isUserInvited(Client* client) {
	int clientFd = client->getSocket().getFd();

	for (std::vector<Client*>::iterator it = inviteList.begin();
		 it != inviteList.end(); ++it) {
		if ((*it)->getSocket().getFd() == clientFd)
			return 1;
	}
	return 0;
}

void Channel::setInviteMode(void) { inviteOnly = 1; }

void Channel::unsetInviteMode(void) { inviteOnly = 0; }

bool Channel::isInviteMode(void) { return inviteOnly; }

const std::string& Channel::getTopic(void) const { return topic; }

void Channel::changeTopic(const std::string& newTopic) { topic = newTopic; }

void Channel::unsetTopic(void) { topic = ""; }

void Channel::lockTopic(void) { this->topicLocked = 1; }

void Channel::unlockTopic(void) { this->topicLocked = 0; }

void Channel::setChannelPassword(const std::string& newPassword) {
	if (channelPassword.size() == 0) {
		throw std::logic_error("Bad password length");
	}
	channelPassword = newPassword;
}

void Channel::unsetChannelPassword(void) { channelPassword = ""; }

const std::string& Channel::getChannelPassword(void) const {
	return channelPassword;
}

void Channel::addUser(Client* user) { usersOnChannel.push_back(user); }

void Channel::removeUser(Client* user) {
	std::vector<Client*>::iterator it =
		std::find(usersOnChannel.begin(), usersOnChannel.end(), user);
	usersOnChannel.erase(it);
}

void Channel::addOperator(Client* newOp) {
	if (std::find(operators.begin(), operators.end(), newOp) !=
		operators.end()) {
		throw std::logic_error("User is already an operator");
	}
	operators.push_back(newOp);
}

void Channel::removeOperator(Client* oldOp) {
	if (std::find(operators.begin(), operators.end(), oldOp) ==
		operators.end()) {
		throw std::logic_error("User is not an operator");
	}
	operators.erase(std::find(operators.begin(), operators.end(), oldOp));
}

size_t Channel::getChannelSize(void) { return channelSize; }

void Channel::changeChannelSize(size_t newSize) {
	if (newSize == 0) {
		throw std::logic_error("Bad channel size");
	}
	channelSize = newSize;
};

void Channel::unsetSize(void) { channelSize = 0; };

const char* Channel::InvalidChannelPrefixException::what() const throw() {
	return "Invalid channel prefix, you may only use  #";
}

const char* Channel::ForbiddenChannelNameException::what() const throw() {
	if (ch == ' ')
		return "Character ' ' is forbidden in channel name";
	if (ch == 0x07)
		return "Character '^G' (BEL) is forbidden in channel name";
	if (ch == ',')
		return "Character ',' is forbidden in channel name";
	else
		return "Forbidden character used in channel name";
}

void Channel::sendMessage(const std::string& prefix,
						  const std::string& trailing) {
	for (std::vector<Client*>::iterator it = usersOnChannel.begin();
		 it != usersOnChannel.end(); it++) {
		try {
			(*it)->sendMessage(prefix, "PRIVMSG", name, trailing);
		} catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}
}

void Channel::inviteUser(Client* user) { inviteList.push_back(user); }

void Channel::uninviteUser(Client* user) {
	std::vector<Client*>::iterator it =
		std::find(inviteList.begin(), inviteList.end(), user);
	if (it == inviteList.end()) {
		throw Server::ClientNotFoundException();
	}
	inviteList.erase(it);
}
