#include "Channel.hpp"
#include <stdexcept>
#include <algorithm>

void	checkChannelSyntax(std::string channelName) {
	if (channelName[0] != '#')
	{
		throw Channel::InvalidChannelPrefixException();
	}

	if (channelName.find(" ") != channelName.npos)
	{
		throw Channel::ForbiddenChannelNameException(' ');
	}
	if (channelName.find(0x07) != channelName.npos)
	{
		throw Channel::ForbiddenChannelNameException(0x07);
	}
	if (channelName.find(",") != channelName.npos)
	{
		throw Channel::ForbiddenChannelNameException(',');
	}
}

Channel::Channel(Client* creator, std::string name) : name(name) {
	checkChannelSyntax(name);
	operators = {creator};
	usersOnChannel = {creator};
	topic = "";
	inviteOnly = 0;
	channelSize = DEF_CHAN_SIZE;
}

Channel::Channel(Client* creator, std::string name, std::string password) : name(name), channelPassword(password) {
	checkChannelSyntax(name);
	operators = {creator};
	usersOnChannel = {creator};
	topic = "";
	inviteOnly = 0;
	channelSize = DEF_CHAN_SIZE;
}

Channel::~Channel() {}

const std::string& Channel::getChannelName(void) {
	return name;
}

bool	Channel::isUserOnChannel(Client* client) {
	int	clientFd = client->getSocket().getFd();

	for (std::vector<Client*>::iterator it = usersOnChannel.begin(); it != usersOnChannel.end(); ++it) {
		if ((*it)->getSocket().getFd() == clientFd)
			return 1;
	}
	return 0;
}

bool	Channel::isUserOperator(Client* client) {
	if (!Channel::isUserOnChannel(client))
		return 0;

	int	clientFd = client->getSocket().getFd();

	for (std::vector<Client*>::iterator it = operators.begin(); it != operators.end(); ++it) {
		if ((*it)->getSocket().getFd() == clientFd)
			return 1;
	}
	return 0;
}

void	Channel::setInviteMode(void) {
	inviteOnly = 1;
}

void	Channel::unsetInviteMode(void) {
	inviteOnly = 0;
}

void	Channel::changeTopic(std::string newTopic) {
	topic = newTopic;
}

void	Channel::unsetTopic(void) {
	topic = "";
}

void	Channel::lockTopic(void) {
	this->topicLocked = 1;
}

void	Channel::unlockTopic(void) {
	this->topicLocked = 0;
}

void	Channel::setChannelPassword(std::string newPassword) {
	if (channelPassword.size() == 0)
	{
		throw std::logic_error("Bad password length");
		return ;
	}
	channelPassword = newPassword;
}

void	Channel::unsetChannelPassword(void) {
	channelPassword = "";
}

void	Channel::addOperator(Client* newOp) {
	if (std::find(operators.begin(), operators.end(), newOp) != operators.end())
	{
		throw std::logic_error("User is already an operator");
		return ;
	}
	operators.push_back(newOp);
}

void	Channel::removeOperator(Client* oldOp) {
	if (std::find(operators.begin(), operators.end(), oldOp) == operators.end())
	{
		throw std::logic_error("User is not an operator");
		return ;
	}
	operators.erase(std::find(operators.begin(), operators.end(), oldOp));
}

void	Channel::changeChannelSize(size_t newSize) {
	if (newSize == 0)
	{
		throw std::logic_error("Bad channel size");
		return ;
	}
	channelSize = newSize;
};

void	Channel::unsetSize(void) {
	channelSize = 0;
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
