#pragma once

#include "Client.hpp"
#include "Command.hpp"

#include <string>
#include <vector>

class ModeCommand : public Command {
  private:
	std::vector<std::string> args;

	bool checkModes(Client* client) const;
	void parseModes(Client* client, Channel* channel);
	void parseModes(Client* client);
	void setMode(Client* client, bool action, char mode,
				 const std::string& param = "");
	void setMode(Client* client, Channel* channel, bool action, char mode,
				 const std::string& param = "");

  public:
	ModeCommand();
	~ModeCommand();

	void execute(Client* client, std::string args);
};
