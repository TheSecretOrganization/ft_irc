#pragma once

#include <string>
#include <vector>

#include "Client.hpp"
#include "ServerListener.hpp"
#include "SocketObserver.hpp"


class Server {

  private:
	std::vector<Client*> clients;
	SocketObserver observer;
	ServerListener listener;
	std::string password;
	bool run;

	Server();

  public:
	~Server();

	static Server& getInstance();
	void start(int port, const std::string& password);
	void shut();
	void addClient(Client* client);
};
