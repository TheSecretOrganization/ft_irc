#pragma once

#include "ClientSocket.hpp"

#include <string>

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
	size_t joinedChannels;

  public:
	Client(int fd);
	~Client();

	void sendMessage(std::string type, std::string message) const;

	ClientSocket& getSocket();
	std::string getClientnickName(void);
	std::string getNickname() const;
	void setNickname(const std::string& newNickname);
	std::string getUsername() const;
	void setUsername(const std::string& newUsername);
	std::string getRealname() const;
	void setRealname(const std::string& newRealname);
	std::string getHostname() const;
	void setHostname(const std::string& newHostname);
	std::string getServername() const;
	void setServername(const std::string& newServername);
	int getStatus() const;
	void setStatus(int newStatus);
	size_t getJoinedChannels();
	void resetJoinedChannels();
	void incrementJoinedChannels();
	void decrementJoinedChannels();
};
