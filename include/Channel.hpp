#pragma once

#include "Client.hpp"
#include "Command.hpp"

#include <cstddef>
#include <ctime>
#include <string>
#include <vector>

class Command;

typedef struct sTopic {
	std::string content;
	bool locked;
	std::time_t setAt;
	std::string setBy;

	sTopic() : content(""), locked(false), setAt(0), setBy("") {}
} topic_t;

class Channel {
  private:
	std::string name;
	std::string password;
	std::vector<Client*> operators;
	std::vector<Client*> usersOnChannel;
	std::vector<Client*> inviteList;
	std::vector<std::string> bans;
	bool inviteOnly;
	size_t userLimit;
	topic_t topic;

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
	std::vector<std::string>& getBans();

	const std::string& getName(void) const;
	bool isUserOnChannel(Client* client);
	bool isUserOperator(Client* client);
	bool isUserInvited(Client* client);
	bool isUserBanned(Client* client) const;

	void setInviteMode(bool newInviteMode);
	bool isInviteMode(void) const;

	const std::string& getTopic() const;
	void setTopic(Client* client, const std::string& newTopic);
	void setTopicLocked(bool newTopicLocked);
	bool isTopicLocked() const;

	void setPassword(const std::string& newPassword);
	void unsetPassword(void);
	const std::string& getPassword(void) const;

	void addUser(Client* user);
	void removeUser(Client* user);

	void addOperator(Client* newOp);
	void removeOperator(Client* oldOp);

	size_t getUserLimit(void) const;
	void setUserLimit(size_t newUserLimit);

	void broadcast(const std::string& prefix, const std::string& command,
				   const std::string& parameter = "",
				   const std::string& trailing = "");

	void inviteUser(Client* user);
	void uninviteUser(Client* user);

	std::string getModes(Client* user = NULL);

	void rplTopic(Client* client) const;
	void rplNoTopic(Client* client) const;
	void rplTopicWhoTime(Client* client) const;

	void addBan(const std::string& ban);
	void deleteBan(const std::string& ban);
	bool checkBanSyntax(const std::string& ban) const;

	void kick(Client* client, Client* user, const std::string& message) const;
};
