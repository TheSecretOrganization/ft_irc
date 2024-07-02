#include "ClientSocket.hpp"
#include "Server.hpp"
#include "Socket.hpp"

#include <cstddef>
#include <cstdio>
#include <exception>
#include <iostream>
#include <string>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#define SIZE 100

ClientSocket::ClientSocket(int fd) : Socket(fd) {}

ClientSocket::~ClientSocket() {}

void ClientSocket::onPoll() {
	char buff[SIZE];
	std::string content;

	bzero(buff, SIZE);
	ssize_t size;
	while ((size = recv(fd, buff, SIZE, 0)) != 0) {
		if (size == -1)
			break;

		content.append(buff);
		bzero(buff, SIZE);
	}

	if (content.empty() || content.size() < 2 ||
		content[content.size() - 2] != '\r' ||
		content[content.size() - 1] != '\n') {
		try {
			std::cout << "Killing Client" << std::endl;
			Server::getInstance().deleteClient(
				Server::getInstance().getClient(fd));
		} catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
		return;
	}

	content = content.substr(0, content.size() - 2);

	size_t cs;
	do {
		cs = content.find("\n");
		size_t i = cs == std::string::npos ? content.size() + 1 : cs;
		std::string command = content.substr(0, i - 1);
		content = content.erase(0, i + 1);

		i = command.find(" ");
		std::string name = command.substr(0, i);
		command = (i != std::string::npos)
					  ? command.substr(i + 1, command.size() - (i + 1))
					  : "";
		try {
			Command* cmd =
				Server::getInstance().getCommandRegistry().getCommand(name);
			cmd->execute(Server::getInstance().getClient(fd), command);
		} catch (CommandRegistry::NotFoundException& e) {
			std::cerr << name << ": " << e.what() << std::endl;
		} catch (Server::ClientNotFoundException& e) {
			std::cerr << "client not found" << std::endl;
		}
	} while (cs != std::string::npos);
}

void ClientSocket::sendPacket(const std::string& packet) const {
	std::string formatPacket = ":" + packet + "\r\n";
	if (send(fd, formatPacket.c_str(), formatPacket.size(), 0) == -1)
		throw SendException();
}

const char* ClientSocket::SendException::what() const throw() {
	return "cannot send packet to client";
}
