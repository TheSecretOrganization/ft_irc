#pragma once

#include "Command.hpp"

class CapCommand : public Command {
  private:
	void rplWelcome(Client* client) const;
	void rplYourHost(Client* client) const;
	void rplCreated(Client* client) const;
	void rplMyInfo(Client* client) const;
	void rplISupport(Client* client) const;

  public:
	CapCommand();
	~CapCommand();

	void execute(Client* client, std::string args);
};
