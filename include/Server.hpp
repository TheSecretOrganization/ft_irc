#pragma once

#include <string>
#include <vector>

#include "Client.hpp"
#include "CommandRegistry.hpp"
#include "ServerSocket.hpp"
#include "SocketObserver.hpp"

class Server {

  private:
	std::vector<Client*> clients;
	SocketObserver observer;
	ServerSocket socket;
	CommandRegistry commandRegistry;
	std::string password;
	bool run;

	Server();

  public:
	~Server();

	static Server& getInstance();
	CommandRegistry &getCommandRegistry();
	void start(int port, const std::string& password);
	void shut();
	void addClient(Client* client);
	Client *getClient(int fd);
};
