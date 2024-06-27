#pragma once

#include "Command.hpp"

class CapCommand : public Command {
  private:
	void rplWelcome(Client* client);
	void rplYourHost(Client* client);
	void rplCreated(Client* client);
	void rplMyInfo(Client* client);
	void rplISupport(Client* client);

  public:
	CapCommand();
	~CapCommand();

	void execute(Client* client, std::string args);
};
