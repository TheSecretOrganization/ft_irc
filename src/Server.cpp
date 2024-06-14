#include "Server.hpp"
#include "Client.hpp"
#include "ServerListener.hpp"

#include <vector>

Server::Server() { run = true; }

Server::~Server() {
	for (std::vector<Client*>::iterator it = clients.begin();
		 it != clients.end(); ++it) {
		observer.unsubscribe((*it)->getFd());
		delete *it;
	}

	observer.unsubscribe(listener.getFd());
}

Server& Server::getInstance() {
	static Server instance;
	return instance;
}

void Server::start(int port, const std::string& password) {
	this->password = password;
	listener.init(port);
	observer.subscribe(listener.getFd(), listener);

	while (run) {
		observer.poll();
	}
}

void Server::shut() { run = false; }

void Server::addClient(Client* client) {
	clients.push_back(client);
	observer.subscribe(client->getFd(), *client);
}
