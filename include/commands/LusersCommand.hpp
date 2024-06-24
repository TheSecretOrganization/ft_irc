#pragma once

#include "Client.hpp"
#include "Command.hpp"

#include <string>

class LusersCommand : public Command {

  public:
	LusersCommand();
	~LusersCommand();

	void execute(Client* client, std::string args);
	void rplLuserClient(Client* client) const;
	void rplLuserMe(Client* client) const;
};
