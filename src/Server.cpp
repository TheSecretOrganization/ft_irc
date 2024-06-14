#include "Server.hpp"
#include "Client.hpp"
#include "ServerSocket.hpp"

#include <vector>

Server::Server() { run = true; }

Server::~Server() {
	for (std::vector<Client*>::iterator it = clients.begin();
		 it != clients.end(); ++it) {
		observer.unsubscribe((*it)->getFd());
		delete *it;
	}

	observer.unsubscribe(socket.getFd());
}

Server& Server::getInstance() {
	static Server instance;
	return instance;
}

void Server::start(int port, const std::string& password) {
	socket.init(port, password);
	observer.subscribe(socket.getFd(), socket);

	while (run) {
		observer.poll();
	}
}

void Server::setRun(bool newState) { run = newState; }

void Server::addClient(Client* client) {
	clients.push_back(client);
	observer.subscribe(client->getFd(), *client);
}
