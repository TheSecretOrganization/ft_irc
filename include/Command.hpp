#pragma once

#include "Client.hpp"

#include <cstddef>
#include <string>
#include <vector>

class Command {
  protected:
	Command(const std::string& name, size_t expectedSize, size_t minSize);

	std::string name;
	size_t expectedSize;
	size_t minSize;

  public:
	virtual ~Command();

	virtual void execute(Client* client, std::string args) = 0;
	void sendError(Client* client, std::string code, std::string meassage,
				   std::string arg = "") const;
	static std::vector<std::string> split(const std::string& str, char del);

	virtual bool needMoreParams(Client* client,
								const std::vector<std::string>& vecArgs) const;
	virtual bool alreadyRegistred(Client* client) const;
	virtual bool passwdMismatch(Client* client,
								const std::string& passWd) const;
};
