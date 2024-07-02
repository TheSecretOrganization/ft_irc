#pragma once

#include "Client.hpp"
#include "Command.hpp"

#include <string>

class NamesCommand : public Command {
  private:
	std::string getNames(Client* client, Channel* channel) const;

  public:
	NamesCommand();
	~NamesCommand();

	void execute(Client* client, const std::string& args);
};
