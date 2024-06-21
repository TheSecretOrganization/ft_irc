#include "Server.hpp"
#include "Client.hpp"
#include "ServerSocket.hpp"

#include <algorithm>
#include <iostream>
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
	configuration.setPassword(password);
	socket.init(port);
	observer.subscribe(socket.getFd(), socket);

	while (run) {
		observer.poll();
	}
}

void Server::shut() {
	run = false;
	std::cout << "Bye!" << std::endl;
}

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

void Server::addChannel(Channel* channel) {
	channels.push_back(channel);
}

void Server::deleteChannel(Channel* channel) {
	std::vector<Channel*>::iterator it = std::find(channels.begin(), channels.end(), channel);
	if (it == channels.end())
		throw ChannelNotFoundException();
	channels.erase(it);
	delete channel;
}

const std::vector<Client*>& Server::getClients() const { return clients; }

const Configuration& Server::getConfiguration() const { return configuration; }

bool Server::getRun() const { return run; }

const char* Server::ClientNotFoundException::what() const throw() {
	return "client not found";
}


const char* Server::ChannelNotFoundException::what() const throw() {
	return "channel not found";
}

Client* Server::getClient(std::string nickname) {
	for (std::vector<Client*>::iterator it = clients.begin();
		 it != clients.end(); it++) {
		if ((*it)->getClientnickName() == nickname)
			return *it;
	}
	return NULL;
}

Channel* Server::getChannel(std::string name) {
	for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); it++) {
		if ((*it)->getChannelName() == name)
			return (*it);
	}
	return NULL;
}
