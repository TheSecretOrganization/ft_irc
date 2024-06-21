#pragma once

#include "Command.hpp"
#include <vector>

#define DEF_CHAN_SIZE 3

class Command;

class Channel {
private:
	std::string	name;
	std::string	channelPassword;
	std::vector<Client*>	operators;
	std::vector<Client*>	usersOnChannel;
	std::vector<Client*>	inviteList;
	std::string	topic;
	bool	inviteOnly;
	bool	topicLocked;
	size_t	channelSize;
public:
	Channel(Client* creator, std::string name);
	Channel(Client* creator, std::string name, std::string password);
	~Channel();

	static void	createChannel(Client* client, std::string name, std::string password);

	static void	checkChannelSyntax(std::string channelName);

	class InvalidChannelPrefixException : public std::exception {
	  public:
		virtual const char* what() const throw();
	};

	class ForbiddenChannelNameException : public std::exception {
	  private:
		char ch;
	  public:
		ForbiddenChannelNameException(char ch) : ch(ch) {};
		virtual const char* what() const throw();
	};

	std::vector<Client*>&	getUsers(void);
	std::vector<Client*>&	getOperators(void);
	std::vector<Client*>&	getInviteList(void);

	const std::string& getChannelName(void);
	bool	isUserOnChannel(Client* client);
	bool	isUserOperator(Client* client);
	bool	isUserInvited(Client* client);

	void	setInviteMode(void);
	void	unsetInviteMode(void);
	bool	isInviteMode(void);

	void	changeTopic(std::string newTopic);
	void	unsetTopic(void);
	void	lockTopic(void);
	void	unlockTopic(void);

	void	setChannelPassword(std::string newPassword);
	void	unsetChannelPassword(void);
	const std::string& getChannelPassword(void);

	void	addUser(Client* user);
	void	removeUser(Client* user);

	void	addOperator(Client* newOp);
	void	removeOperator(Client *oldOp);

	size_t	getChannelSize(void);
	void	changeChannelSize(size_t newSize);
	void	unsetSize(void);
};
