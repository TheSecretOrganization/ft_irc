#include "commands/QuitCommand.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "Server.hpp"

#include <string>

QuitCommand::QuitCommand(): Command("QUIT", 0, 0) {}

QuitCommand::~QuitCommand() {}

void QuitCommand::execute(Client* client, std::string args) {
    if (args.empty())
        args = "left the server";

    std::string msg = "Quit: " + client->getNickname() + " " + args;

    //TODO: send message to other clients

    Server::getInstance().getCommandRegistry().getCommand("ERROR")->execute(client, "Quit: Bye for now!");
}
