#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "ServerSocket.hpp"

#include <algorithm>
#include <iostream>
#include <vector>

Server::Server() { run = true; }

Server::~Server() {
	for (std::vector<Client*>::iterator it = clients.begin();
		 it != clients.end(); ++it) {
		if ((*it)->isBot())
			continue;
		observer.unsubscribe((*it)->getSocket().getFd());
		delete *it;
	}

	delete guardian;

	try {
		observer.unsubscribe(socket.getFd());
	} catch (SocketObserver::EpollCtlDelException& e) {
		std::cerr << e.what() << std::endl;
	}

	for (std::vector<Channel*>::iterator it = channels.begin();
		 it != channels.end(); it++) {
		delete *it;
	}
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

	guardian = new Bot();
	clients.push_back(guardian);

	while (run) {
		observer.poll();
	}
}

void Server::shut() {
	run = false;
	std::cout << "\nBye!" << std::endl;
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

void Server::addChannel(Channel* channel) { channels.push_back(channel); }

void Server::deleteChannel(Channel* channel) {
	std::vector<Channel*>::iterator it =
		std::find(channels.begin(), channels.end(), channel);
	if (it == channels.end())
		throw ChannelNotFoundException();
	channels.erase(it);
	delete channel;
}

bool Server::deleteIfGhostChannel(Channel* channel) {
	if (channel->getUsers().size() - 1 == 0) {
		try {
			deleteChannel(channel);
		} catch (const Server::ChannelNotFoundException& e) {
			std::cerr << e.what() << std::endl;
		}
		return true;
	}
	return false;
}

std::string Server::getPrefix() const {
	return getConfiguration().getValue("serverName");
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

Client* Server::getClient(const std::string& nickname) {
	for (std::vector<Client*>::iterator it = clients.begin();
		 it != clients.end(); it++) {
		if ((*it)->getClientnickName() == nickname)
			return *it;
	}
	return NULL;
}

Channel* Server::getChannel(const std::string& name) {
	for (std::vector<Channel*>::iterator it = channels.begin();
		 it != channels.end(); it++) {
		if ((*it)->getName() == name)
			return (*it);
	}
	return NULL;
}

const std::vector<Channel*>& Server::getChannels() const { return channels; }

std::vector<Channel*> Server::getChannels(Client* client) const {
	std::vector<Channel*> clientChannels;

	for (size_t i = 0; i < channels.size(); i++) {
		if (channels[i]->isUserOnChannel(client))
			clientChannels.push_back(channels[i]);
	}

	return clientChannels;
}

Bot* Server::getGuardian() { return guardian; }
