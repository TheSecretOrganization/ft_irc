#pragma once

#include "Client.hpp"
#include <string>
#include <string>
#include <vector>

enum {
	USER = 1
};

class Command {

  public:
	virtual ~Command(){};

	virtual void execute(Client* client, std::string args) = 0;
	void sendError(Client* client, std::string code, std::string meassage,
				   std::string arg = "") const;
	std::vector<std::string> split(const std::string& str, char del) const;

	class NeedMoreParamsException : public std::exception {
	  private:
		const Client* client;
		const int command;

	  public:
		NeedMoreParamsException(const Client* client, int command);
		virtual const char* what() const throw();
	};
};
