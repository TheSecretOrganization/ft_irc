#pragma once

#include "Channel.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include <string>

class NickCommand : public Command {
  private:
	std::vector<Client*> getAffectedUsers(Client* client) const;

  public:
	NickCommand();
	~NickCommand();

	void execute(Client* client, const std::string& args);
};
