#pragma once

#include "Client.hpp"
#include "Command.hpp"

#include <string>
#include <vector>

class ModeCommand : public Command {
  private:
	std::vector<std::string> splitArgs;

	bool checkModes(Client* client) const;
	void parseModes(Client* client, Channel* channel) const;
	void parseModes(Client* client) const;
	void setMode(Client* client, bool action, char mode,
				 const std::string& param = "") const;
	void setMode(Client* client, Channel* channel, bool action, char mode,
				 const std::string& param = "") const;
	void setBan(Client* client, Channel* channel, bool action,
				const std::string& ban) const;
	void sendBanList(Client* client, Channel* channel) const;

  public:
	ModeCommand();
	~ModeCommand();

	void execute(Client* client, const std::string& args);
};
