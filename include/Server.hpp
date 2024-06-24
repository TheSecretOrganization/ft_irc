#pragma once

#include <string>
#include <vector>

#include "Channel.hpp"
#include "Client.hpp"
#include "ClientCommands.hpp"
#include "CommandRegistry.hpp"
#include "Configuration.hpp"
#include "ServerCommands.hpp"
#include "ServerSocket.hpp"
#include "SocketObserver.hpp"

class Server {

  private:
	std::vector<Client*> clients;
	std::vector<Channel*> channels;
	SocketObserver observer;
	ServerSocket socket;
	ClientCommands clientCommands;
	ServerCommands serverCommands;
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
	CommandRegistry& getClientCommands();
	CommandRegistry& getServerCommands();
	Client* getClient(int fd);
	const std::vector<Client*>& getClients() const;
	const Configuration& getConfiguration() const;
	bool getRun() const;
	Client* getClient(std::string nickname);
	Channel* getChannel(std::string name);
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
