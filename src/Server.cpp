#include "Server.hpp"
#include "Client.hpp"
#include "ServerSocket.hpp"

#include <algorithm>
#include <vector>

Server::Server() { run = true; }

Server::~Server() {
	for (std::vector<Client*>::iterator it = clients.begin();
		 it != clients.end(); ++it) {
		observer.unsubscribe((*it)->getSocket().getFd());
		delete *it;
	}

	observer.unsubscribe(socket.getFd());
}

Server& Server::getInstance() {
	static Server instance;
	return instance;
}

CommandRegistry& Server::getCommandRegistry() { return commandRegistry; }

void Server::start(int port, const std::string& password) {
	this->password = password;
	socket.init(port);
	observer.subscribe(socket.getFd(), socket);

	while (run) {
		observer.poll();
	}
}

void Server::shut() { run = false; }

void Server::addClient(Client* client) {
	clients.push_back(client);
	observer.subscribe(client->getSocket().getFd(), client->getSocket());
}

Client* Server::getClient(int fd) {
	for (std::vector<Client*>::iterator it = clients.begin();
		 it != clients.end(); it++) {
		if ((*it)->getSocket().getFd() == fd)
			return *it;
	}
	throw ClientNotFoundException();
}

void Server::deleteClient(Client* client) {
	std::vector<Client*>::iterator it =
		std::find(clients.begin(), clients.end(), client);
	if (it == clients.end())
		throw ClientNotFoundException();
	observer.unsubscribe((*it)->getSocket().getFd());
	delete *it;
	clients.erase(it);
}

const char* Server::ClientNotFoundException::what() const throw() {
	return "client not found";
}
