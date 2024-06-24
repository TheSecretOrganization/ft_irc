#include "CommandRegistry.hpp"

CommandRegistry::CommandRegistry() {}

CommandRegistry::~CommandRegistry() {
	for (std::map<std::string, Command*>::iterator it = commands.begin();
		 it != commands.end(); it++)
		delete it->second;
}

static std::string strtolower(const std::string& str) {
	std::string lstr = str;
	for (size_t i = 0; i < lstr.size(); i++)
		lstr[i] = tolower(lstr[i]);
	return lstr;
}

void CommandRegistry::registerCommand(const std::string& name,
									  Command* command) {
	std::string lowerName = strtolower(name);
	if (commands.find(lowerName) != commands.end())
		throw DuplicateException();
	commands.insert(std::pair<std::string, Command*>(lowerName, command));
}

Command* CommandRegistry::getCommand(const std::string& name) const {
	std::map<std::string, Command*>::const_iterator cmd =
		commands.find(strtolower(name));
	if (cmd == commands.end())
		throw NotFoundException();
	return cmd->second;
}

const char* CommandRegistry::DuplicateException::what() const throw() {
	return "duplicate exception";
}

const char* CommandRegistry::NotFoundException::what() const throw() {
	return "not found exception";
}
