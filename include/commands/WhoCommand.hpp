#pragma once

#include "Client.hpp"
#include "Command.hpp"
#include <string>

class WhoCommand : public Command {
  private:
	std::string getUserInfo(Client* user, bool isUserOperator) const;
	void channel(Client* client, const std::string& args) const;
	void user(Client* client, const std::string& args) const;

  public:
	WhoCommand();
	~WhoCommand();

	void execute(Client* client, const std::string& args);
};
