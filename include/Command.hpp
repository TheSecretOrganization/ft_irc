#pragma once

#include "Client.hpp"
#include "Channel.hpp"

#include <string>
#include <vector>

class Command {
  protected:
	static std::string	command;
	static size_t		expectedSize;

  public:
	virtual ~Command(){};

	virtual void execute(Client* client, std::string args) = 0;
	void sendError(Client* client, std::string code, std::string meassage,
				   std::string arg = "") const;
	std::vector<std::string> split(const std::string& str, char del) const;

	virtual bool	needMoreParams(Client* client, std::vector<std::string>& vecArgs);
	virtual bool	noSuchChannel(Client* client, Channel* channel, std::string channelName);
	virtual bool	notOnChannel(Client* client, Channel* channel);
	virtual bool	userOnChannel(Client* client, Channel* channel, std::string nick);
};
