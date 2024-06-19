#pragma once

#include "Client.hpp"
#include "Command.hpp"
#include <string>

class UserCommand : public Command {

  public:
	UserCommand();
	~UserCommand();

	void execute(Client* client, std::string args);

	class AlreadyRegisterException : public std::exception {
	  private:
	  	const Client* client;
	  public:
	  	AlreadyRegisterException(const Client* client);
		virtual const char* what() const throw();
	};
};
