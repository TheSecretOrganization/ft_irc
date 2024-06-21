#pragma once

#include "Channel.hpp"
#include "Client.hpp"

#include <cstddef>
#include <string>
#include <vector>

class Channel;

class Command {
  protected:
	Command(const std::string& name, size_t expectedSize, size_t minSize);

	std::string name;
	size_t expectedSize;
	size_t minSize;

  public:
	virtual ~Command();

	virtual void execute(Client* client, std::string args) = 0;
	void sendError(Client* client, std::string code, std::string message,
				   std::string arg = "") const;
	static std::vector<std::string> split(const std::string& str, char del);

	virtual bool alreadyRegistred(Client* client) const;
	virtual bool passwdMismatch(Client* client,
								const std::string& passWd) const;

	virtual bool needMoreParams(Client* client,
								const std::vector<std::string>& vecArgs) const;
	virtual bool noSuchChannel(Client* client, Channel* channel,
							   std::string channelName) const;
	virtual bool notOnChannel(Client* client, Channel* channel) const;
	virtual bool userOnChannel(Client* client, Channel* channel,
							   std::string nick) const;
};
