#pragma once

#include <string>
#include <vector>

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

	Server();

  public:
	~Server();

	void start(int port, const std::string& password);
	void shut();
	void addClient(Client* client);
	void deleteClient(Client* client);
	void addChannel(Channel* channel);
	void deleteChannel(Channel* channel);

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

	class ClientNotFoundException : public std::exception {
	  public:
		virtual const char* what() const throw();
	};
	class ChannelNotFoundException : public std::exception {
	  public:
		virtual const char* what() const throw();
	};
};
