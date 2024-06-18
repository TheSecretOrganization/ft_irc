#pragma once

#include "Client.hpp"

class Command {

  public:
	virtual ~Command(){};

	virtual void execute(Client* client, std::string args) = 0;

	virtual void splitArgs(std::string args, std::vector<std::string>* vstr);

	class MissingParamsInCommandException : public std::exception {
	  private:
		Client* client;
		std::string command;
	  public:
		MissingParamsInCommandException(Client* client, std::string command) : client(client), command(command) {};
		virtual const char* what() const throw();
	};

	class NoChannelException : public std::exception {
	  private:
		Client* client;
		std::string channel;
	  public:
		NoChannelException(Client* client, std::string channel) : client(client), channel(channel) {};
		virtual const char* what() const throw();
	};

	class NotOnChannelException : public std::exception {
	  private:
		Client* client;
		std::string channel;
	  public:
		NotOnChannelException(Client* client, std::string channel) : client(client), channel(channel) {};
		virtual const char* what() const throw();
	};

	class UserOnChannelException : public std::exception {
	  private:
		Client* client;
		std::string nick;
		std::string channel;
	  public:
		UserOnChannelException(Client* client, std::string nick, std::string channel) : client(client), nick(nick), channel(channel) {};
		virtual const char* what() const throw();
	};
};
