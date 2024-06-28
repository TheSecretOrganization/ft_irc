#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

#include <algorithm>
#include <exception>
#include <iostream>
#include <sstream>
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

Channel::Channel(Client* creator, const std::string& name)
	: name(name), password("") {
	checkChannelSyntax(name);
	operators.push_back(creator);
	usersOnChannel.push_back(creator);
	topic = "";
	topicLocked = false;
	inviteOnly = 0;
	userLimit = 0;
}

Channel::Channel(Client* creator, const std::string& name,
				 const std::string& password)
	: name(name), password(password) {
	checkChannelSyntax(name);
	operators.push_back(creator);
	usersOnChannel.push_back(creator);
	topic = "";
	topicLocked = false;
	inviteOnly = 0;
	userLimit = 0;
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

const std::string& Channel::getName(void) const { return name; }

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

void Channel::setInviteMode(bool newInviteMode) { inviteOnly = newInviteMode; }

bool Channel::isInviteMode(void) const { return inviteOnly; }

const std::string& Channel::getTopic(void) const { return topic; }

void Channel::changeTopic(const std::string& newTopic) { topic = newTopic; }

void Channel::unsetTopic(void) { topic = ""; }

void Channel::setTopicLocked(bool newTopicLocked) {
	topicLocked = newTopicLocked;
}

bool Channel::isTopicLocked() const { return topicLocked; }

void Channel::setPassword(const std::string& newPassword) {
	if (password.size() == 0) {
		throw std::logic_error("Bad password length");
	}
	password = newPassword;
}

void Channel::unsetPassword(void) { password = ""; }

const std::string& Channel::getPassword(void) const { return password; }

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

size_t Channel::getUserLimit(void) { return userLimit; }

void Channel::setUserLimit(size_t newUserLimit) {
	if (newUserLimit == 0) {
		throw std::logic_error("Bad channel size");
	}
	userLimit = newUserLimit;
};

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

void Channel::broadcast(const std::string& prefix,
						const std::string& trailing) {
	for (std::vector<Client*>::iterator it = usersOnChannel.begin();
		 it != usersOnChannel.end(); it++) {
		if (prefix == (*it)->getPrefix())
			continue;
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

static std::string size_tToString(size_t value) {
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

std::string Channel::getModes(Client* client) {
	std::string modes = "";
	std::string keys = "";

	if (inviteOnly)
		modes += 'i';

	if (topicLocked)
		modes += 't';

	if (!password.empty()) {
		modes += 'k';
		keys += ' ' + password;
	}

	if (userLimit != 0) {
		modes += 'l';
		keys += ' ' + size_tToString(userLimit);
	}

	return modes + (client && isUserOperator(client) ? keys : "");
}
