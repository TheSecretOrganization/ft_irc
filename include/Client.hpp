#pragma once

#include "ClientSocket.hpp"

#include <string>
#include <vector>

class Channel;

enum { UNKNOWN = 0, PASSWD_OK, NICK_OK, USER_OK, REGISTRED };

class Client {

  private:
	ClientSocket socket;
	std::string realname;
	std::string username;
	std::string nickname;
	std::string hostname;
	std::string servername;
	int status;
	bool away;
	bool invisible;

  public:
	Client(int fd);
	~Client();

	void sendMessage(const std::string& prefix, const std::string& command,
					 const std::string& parameters = "",
					 const std::string& trailing = "") const;
	void sendError(const std::string& command,
				   const std::string& parameters = "",
				   const std::string& trailing = "") const;
	std::string getPrefix() const;
	std::string getModes() const;
	std::vector<Channel*> getJoinedChannels() const;

	ClientSocket& getSocket();
	const std::string& getClientnickName(void);
	const std::string& getNickname() const;
	void setNickname(const std::string& newNickname);
	const std::string& getUsername() const;
	void setUsername(const std::string& newUsername);
	const std::string& getRealname() const;
	void setRealname(const std::string& newRealname);
	const std::string& getHostname() const;
	void setHostname(const std::string& newHostname);
	const std::string& getServername() const;
	void setServername(const std::string& newServername);
	int getStatus() const;
	void setStatus(int newStatus);
	bool isAway() const;
	void setAway(bool newAway);
	bool isInvisible() const;
	void setInvisible(bool newInvisible);
};
