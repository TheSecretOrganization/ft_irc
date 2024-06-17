#pragma once

#include <string>
#include <vector>

#include "Client.hpp"
#include "ServerSocket.hpp"
#include "SocketObserver.hpp"

class Server {

  private:
	std::vector<Client*> clients;
	SocketObserver observer;
	ServerSocket socket;
	std::string password;
	bool run;

	Server();

  public:
	~Server();

	static Server& getInstance();
	void start(int port, const std::string& password);
	void shut();
	void addClient(Client* client);

	class InvalidPortRangeException : public std::exception {
	  public:
		virtual const char* what() const throw();
	};

	class InvalidArgumentNumberException : public std::exception {
	  public:
		virtual const char* what() const throw();
	};
};
