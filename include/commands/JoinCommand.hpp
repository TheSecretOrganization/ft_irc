#pragma once

#include "Command.hpp"

#include <map>

class JoinCommand : public Command {
  private:
	std::vector<Channel*>
	getTrueChannels(Client* client,
					std::map<std::string, std::string>& map) const;
	bool badChannelKey(Client* client, Channel* channel,
					   const std::string& password) const;
	bool isChannelFull(Client* client, Channel* channel) const;
	bool inviteOnlyChan(Client* client, Channel* channel) const;
	std::string getNames(std::vector<Client*>& users) const;
	void sendReplies(Client* client, Channel* channel) const;

  public:
	JoinCommand();
	~JoinCommand();

	void execute(Client* client, std::string args);
};
