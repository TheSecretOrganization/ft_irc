#include "Command.hpp"
#include <exception>
#include <iostream>

void Command::sendError(Client* client, std::string code, std::string message,
						std::string arg) const {
	try {
		if (!arg.empty())
			client->sendMessage(code, arg + " :" + message);
		else
			client->sendMessage(code, ":" + message);
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
