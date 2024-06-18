#pragma once

#include "Client.hpp"
#include "Command.hpp"
#include <string>

class NickCommand : public Command {

  public:
	NickCommand();
	~NickCommand();

	void execute(Client* client, std::string args);

	class NoNicknameGivenException : public std::exception {
	  private:
		Client* client;

	  public:
		NoNicknameGivenException(Client* client);
		virtual const char* what() const throw();
	};

	class ErrOneUsNicknameException : public std::exception {
	  private:
		Client* client;
		const std::string* args;

	  public:
		ErrOneUsNicknameException(Client* client, const std::string* args);
		virtual const char* what() const throw();
	};

	class NicknameInUseException : public std::exception {
	  private:
		Client* client;
		const std::string* args;

	  public:
		NicknameInUseException(Client* client, const std::string* args);
		virtual const char* what() const throw();
	};
};
