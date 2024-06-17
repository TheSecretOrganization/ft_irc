#include "Channel.hpp"
#include <stdexcept>
#include <algorithm>

Channel::Channel(Client* creator, std::string name) : name(name) {
	operators = {creator};
	usersOnChannel = {creator};
	topic = "";
	inviteOnly = 0;
	channelSize = DEF_CHAN_SIZE;
}

Channel::Channel(Client* creator, std::string name, std::string password) : name(name), channelPassword(password) {
	operators = {creator};
	usersOnChannel = {creator};
	topic = "";
	inviteOnly = 0;
	channelSize = DEF_CHAN_SIZE;
}

Channel::~Channel() {}

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
