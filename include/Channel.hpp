#pragma once
#include "Server.hpp"
#include <vector>

#define DEF_CHAN_SIZE 3

class Channel {
private:
	std::string	name;
	std::string	channelPassword;
	std::vector<Client*>	operators;
	std::vector<Client*>	usersOnChannel;
	std::string	topic;
	bool	inviteOnly;
	size_t	channelSize;
public:
	Channel(Client* creator, std::string name);
	Channel(Client* creator, std::string name, std::string password);
	~Channel();

	void	setInviteMode(void);
	void	unsetInviteMode(void);

	void	changeTopic(std::string newTopic);
	void	unsetTopic(void);

	void	setChannelPassword(std::string newPassword);
	void	unsetChannelPassword(void);

	void	addOperator(Client* newOp);
	void	removeOperator(Client *oldOp);

	void	changeChannelSize(size_t newSize);
	void	unsetSize(void);
};
