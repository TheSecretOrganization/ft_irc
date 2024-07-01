#include "Channel.hpp"
#include "Client.hpp"
#include "IrcReplies.hpp"
#include "Server.hpp"

#include <algorithm>
#include <cstddef>
#include <ctime>
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

Channel::Channel(Client* creator, const std::string& name)
	: name(name), password(""), inviteOnly(false), userLimit(0) {
	checkChannelSyntax(name);
	operators.push_back(creator);
	usersOnChannel.push_back(creator);
}

Channel::Channel(Client* creator, const std::string& name,
				 const std::string& password)
	: name(name), password(password), inviteOnly(false), userLimit(0) {
	checkChannelSyntax(name);
	operators.push_back(creator);
	usersOnChannel.push_back(creator);
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

const std::string& Channel::getTopic(void) const { return topic.content; }

void Channel::setTopic(Client* client, const std::string& newTopic) {
	topic.content = newTopic;
	topic.setAt = std::time(NULL);
	topic.setBy = client->getClientnickName();
	broadcast(client->getPrefix(), "TOPIC", topic.content);
}

void Channel::setTopicLocked(bool newTopicLocked) {
	topic.locked = newTopicLocked;
}

bool Channel::isTopicLocked() const { return topic.locked; }

void Channel::setPassword(const std::string& newPassword) {
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

size_t Channel::getUserLimit(void) const { return userLimit; }

void Channel::setUserLimit(size_t newUserLimit) { userLimit = newUserLimit; };

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

void Channel::broadcast(const std::string& prefix, const std::string& command,
						const std::string& parameter,
						const std::string& trailing) {
	for (std::vector<Client*>::iterator it = usersOnChannel.begin();
		 it != usersOnChannel.end(); it++) {
		if (command == "PRIVMSG" && prefix == (*it)->getPrefix())
			continue;
		try {
			(*it)->sendMessage(
				prefix, command,
				name + (parameter.empty() ? "" : " " + parameter), trailing);
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

std::string Channel::getModes(Client* client) {
	std::string modes = "";
	std::string keys = "";

	if (inviteOnly)
		modes += 'i';

	if (topic.locked)
		modes += 't';

	if (!password.empty()) {
		modes += 'k';
		keys += ' ' + password;
	}

	if (userLimit != 0) {
		modes += 'l';
		keys += ' ' + Command::size_tToString(userLimit);
	}

	return modes + (client && isUserOperator(client) ? keys : "");
}

void Channel::rplTopic(Client* client) const {
	client->sendMessage(Server::getInstance().getPrefix(), RPL_TOPIC,
						client->getClientnickName() + " " + name,
						topic.content);
}

void Channel::rplNoTopic(Client* client) const {
	client->sendMessage(Server::getInstance().getPrefix(), RPL_NOTOPIC,
						client->getClientnickName() + " " + name, _331);
}

void Channel::rplTopicWhoTime(Client* client) const {
	client->sendMessage(Server::getInstance().getPrefix(), RPL_TOPICWHOTIME,
						client->getClientnickName() + " " + name + " " +
							topic.setBy + " " +
							Command::size_tToString(topic.setAt));
}
