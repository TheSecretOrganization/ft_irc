#pragma once

#include <string>
#include <vector>

#include "Bot.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "CommandRegistry.hpp"
#include "Configuration.hpp"
#include "ServerSocket.hpp"
#include "SocketObserver.hpp"

class Server {

  private:
	std::vector<Client*> clients;
	std::vector<Channel*> channels;
	SocketObserver observer;
	ServerSocket socket;
	CommandRegistry commandRegistry;
	Configuration configuration;
	bool run;
	Bot* guardian;

	Server();

  public:
	~Server();

	void start(int port, const std::string& password);
	void shut();
	void addClient(Client* client);
	void deleteClient(Client* client);
	void addChannel(Channel* channel);
	void deleteChannel(Channel* channel);
	bool deleteIfGhostChannel(Channel* channel);
	std::string getPrefix() const;

	static Server& getInstance();
	CommandRegistry& getCommandRegistry();
	Client* getClient(int fd);
	const std::vector<Client*>& getClients() const;
	const Configuration& getConfiguration() const;
	bool getRun() const;
	Client* getClient(const std::string& nickname);
	Channel* getChannel(const std::string& name);
	const std::vector<Channel*>& getChannels() const;
	std::vector<Channel*> getChannels(Client* client) const;

	Bot* getGuardian();

	class ClientNotFoundException : public std::exception {
	  public:
		virtual const char* what() const throw();
	};
	class ChannelNotFoundException : public std::exception {
	  public:
		virtual const char* what() const throw();
	};
};
