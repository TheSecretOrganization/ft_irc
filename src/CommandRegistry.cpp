#include "CommandRegistry.hpp"

CommandRegistry::CommandRegistry() {

}

CommandRegistry::~CommandRegistry() {
	for (std::map<std::string, Command *>::iterator it = commands.begin()
	; it != commands.end(); it++)
		delete it->second;
}

void CommandRegistry::registerCommand(std::string &name, Command &command) {
	if (commands.find(name) != commands.end())
		throw DuplicateException();
	commands.insert(std::pair<std::string, Command *>(name, &command));
}

Command *CommandRegistry::getCommand(const std::string &name) const {
	std::map<std::string, Command *>::const_iterator cmd = commands.find(name);
	if (cmd == commands.end())
		throw NotFoundException();
	return cmd->second;
}

const char *CommandRegistry::DuplicateException::what() const throw() {
	return "duplicate exception";
}

const char *CommandRegistry::NotFoundException::what() const throw() {
	return "duplicate exception";
}