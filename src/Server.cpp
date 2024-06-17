#include "Server.hpp"
#include "Client.hpp"
#include "ServerSocket.hpp"

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


const char *Server::InvalidArgumentNumberException::what() const throw() {
	return "Bad argument number";
};

const char *Server::InvalidPortRangeException::what() const throw() {
	return "Invalid port range, use ports between 6660 and 6669 included";
};
