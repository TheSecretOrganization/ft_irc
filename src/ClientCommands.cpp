#include "ClientCommands.hpp"
#include "commands/CapCommand.hpp"
#include "commands/JoinCommand.hpp"
#include "commands/LusersCommand.hpp"
#include "commands/MotdCommand.hpp"
#include "commands/NickCommand.hpp"
#include "commands/PassCommand.hpp"
#include "commands/PingCommand.hpp"
#include "commands/QuitCommand.hpp"
#include "commands/UserCommand.hpp"

ClientCommands::ClientCommands() {
	registerCommand("cap", new CapCommand());
	registerCommand("nick", new NickCommand());
	registerCommand("ping", new PingCommand());
	registerCommand("user", new UserCommand());
	registerCommand("pass", new PassCommand());
	registerCommand("join", new JoinCommand());
	registerCommand("quit", new QuitCommand());
	registerCommand("lusers", new LusersCommand());
	registerCommand("motd", new MotdCommand());
}

ClientCommands::~ClientCommands() {}
