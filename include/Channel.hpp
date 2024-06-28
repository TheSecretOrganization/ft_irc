#pragma once

#include "Client.hpp"
#include "Command.hpp"

#include <cstddef>
#include <string>
#include <vector>

class Command;

class Channel {
  private:
	std::string name;
	std::string password;
	std::vector<Client*> operators;
	std::vector<Client*> usersOnChannel;
	std::vector<Client*> inviteList;
	std::string topic;
	bool inviteOnly;
	bool topicLocked;
	size_t userLimit;

  public:
	Channel(Client* creator, const std::string& name);
	Channel(Client* creator, const std::string& name,
			const std::string& password);
	~Channel();

	static void createChannel(Client* client, const std::string& name,
							  const std::string& password);

	static void checkChannelSyntax(const std::string& channelName);

	class InvalidChannelPrefixException : public std::exception {
	  public:
		virtual const char* what() const throw();
	};

	class ForbiddenChannelNameException : public std::exception {
	  private:
		char ch;

	  public:
		ForbiddenChannelNameException(char ch) : ch(ch){};
		virtual const char* what() const throw();
	};

	std::vector<Client*>& getUsers(void);
	std::vector<Client*>& getOperators(void);
	std::vector<Client*>& getInviteList(void);

	const std::string& getName(void) const;
	bool isUserOnChannel(Client* client);
	bool isUserOperator(Client* client);
	bool isUserInvited(Client* client);

	void setInviteMode(bool newInviteMode);
	bool isInviteMode(void) const;

	const std::string& getTopic() const;
	void changeTopic(const std::string& newTopic);
	void unsetTopic(void);
	void setTopicLocked(bool newTopicLocked);
	bool isTopicLocked() const;

	void setPassword(const std::string& newPassword);
	void unsetPassword(void);
	const std::string& getPassword(void) const;

	void addUser(Client* user);
	void removeUser(Client* user);

	void addOperator(Client* newOp);
	void removeOperator(Client* oldOp);

	size_t getUserLimit(void);
	void setUserLimit(size_t newUserLimit);

	void broadcast(const std::string& prefix, const std::string& trailing = "");

	void inviteUser(Client* user);
	void uninviteUser(Client* user);

	std::string getModes(Client* user = NULL);
};
