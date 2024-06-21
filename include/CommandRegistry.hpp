#pragma once

#include <map>
#include <string>

#include "Command.hpp"

class CommandRegistry {

  private:
	std::map<std::string, Command*> commands;

  public:
	CommandRegistry();
	~CommandRegistry();

	void registerCommand(const std::string& name, Command* command);
	Command* getCommand(const std::string& name) const;

	class NotFoundException : public std::exception {
	  public:
		virtual const char* what() const throw();
	};

	class DuplicateException : public std::exception {
	  public:
		virtual const char* what() const throw();
	};
};
