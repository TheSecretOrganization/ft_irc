#pragma once

#include "Socket.hpp"

#include <exception>

class ServerSocket : public Socket {
  private:
	int port;

	void onPoll();

  public:
	ServerSocket();
	~ServerSocket();

	void init(int port);

	class SocketException : public std::exception {
	  public:
		virtual const char* what() const throw();
	};

	class BindException : public std::exception {
	  public:
		virtual const char* what() const throw();
	};

	class ListenException : public std::exception {
	  public:
		virtual const char* what() const throw();
	};

	class AcceptException : public std::exception {
	  public:
		virtual const char* what() const throw();
	};

	class FcntlException : public std::exception {
	  public:
		virtual const char* what() const throw();
	};
};
