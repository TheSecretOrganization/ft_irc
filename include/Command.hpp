#pragma once

#include "Client.hpp"
#include "Channel.hpp"

#include <string>
#include <vector>

class Command {
  protected:
	std::string	command;
	size_t		expectedSize;
	size_t		minSize;

  public:
	virtual ~Command(){};

	virtual void execute(Client* client, std::string args) = 0;
	void sendError(Client* client, std::string code, std::string message,
				   std::string arg = "") const;
	static std::vector<std::string> split(const std::string& str, char del);
	static size_t stringToSizeT(const std::string& str);

	virtual bool	needMoreParams(Client* client, std::vector<std::string>& vecArgs);
	virtual bool	noSuchChannel(Client* client, Channel* channel, std::string channelName);
	virtual bool	notOnChannel(Client* client, Channel* channel);
	virtual bool	userOnChannel(Client* client, Channel* channel, std::string nick);
};
