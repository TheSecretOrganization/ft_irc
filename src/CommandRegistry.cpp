#include "CommandRegistry.hpp"

#include "commands/CapCommand.hpp"
#include "commands/JoinCommand.hpp"
#include "commands/LusersCommand.hpp"
#include "commands/ModeCommand.hpp"
#include "commands/MotdCommand.hpp"
#include "commands/NickCommand.hpp"
#include "commands/PassCommand.hpp"
#include "commands/PingCommand.hpp"
#include "commands/PrivmsgCommand.hpp"
#include "commands/QuitCommand.hpp"
#include "commands/TopicCommand.hpp"
#include "commands/UserCommand.hpp"

CommandRegistry::CommandRegistry() {
	registerCommand("CAP", new CapCommand());
	registerCommand("NICK", new NickCommand());
	registerCommand("PING", new PingCommand());
	registerCommand("USER", new UserCommand());
	registerCommand("PASS", new PassCommand());
	registerCommand("JOIN", new JoinCommand());
	registerCommand("QUIT", new QuitCommand());
	registerCommand("LUSERS", new LusersCommand());
	registerCommand("MOTD", new MotdCommand());
	registerCommand("PRIVMSG", new PrivmsgCommand());
	registerCommand("MODE", new ModeCommand());
	registerCommand("TOPIC", new TopicCommand());
}

CommandRegistry::~CommandRegistry() {
	for (std::map<std::string, Command*>::iterator it = commands.begin();
		 it != commands.end(); it++)
		delete it->second;
}

static std::string strtoupper(const std::string& str) {
	std::string lstr = str;
	for (size_t i = 0; i < lstr.size(); i++)
		lstr[i] = toupper(lstr[i]);
	return lstr;
}

void CommandRegistry::registerCommand(const std::string& name,
									  Command* command) {
	std::string upperName = strtoupper(name);
	if (commands.find(upperName) != commands.end())
		throw DuplicateException();
	commands.insert(std::pair<std::string, Command*>(upperName, command));
}

Command* CommandRegistry::getCommand(const std::string& name) const {
	std::map<std::string, Command*>::const_iterator cmd =
		commands.find(strtoupper(name));
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
