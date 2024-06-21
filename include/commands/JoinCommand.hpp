#pragma once
#include "Command.hpp"

class JoinCommand : public Command
{
  private:
	std::vector<Channel*> getTrueChannels(Client* client, std::map<std::string, std::string>& map);
	bool	badChannelKey(Client* client, Channel* channel, const std::string& password);
	bool	isChannelFull(Client* client, Channel* channel);
  public:
	JoinCommand();
	~JoinCommand();

	void	execute(Client* client, std::string args);
};
