#include "ClientSocket.hpp"
#include "Server.hpp"
#include "Socket.hpp"

#include <cstdio>
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
	content = content.erase(content.size() - 2, content.size());

	size_t cs;
	do {
		cs = content.find("\n");
		size_t i = cs == std::string::npos ? content.size() + 1 : cs;
		std::string command = content.substr(0, i - 1);
		content = content.erase(0, i + 1);

		i = command.find(" ");
		i = i == std::string::npos ? command.size() + 1 : i;
		std::string name = command.substr(0, i);

		try {
			Command* cmd =
				Server::getInstance().getCommandRegistry().getCommand(name);
			cmd->execute(Server::getInstance().getClient(fd), command);
		} catch (CommandRegistry::NotFoundException& e) {
			std::cout << name << ": " << e.what() << std::endl;
		}
	} while (cs != std::string::npos);
}

void ClientSocket::sendPacket(std::string packet) const {
	packet.append("\r\n");
	if (send(fd, packet.c_str(), packet.size(), 0) == -1) {
		perror("Send message");
		return;
	}
}
