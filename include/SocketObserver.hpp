#pragma once

#include "Socket.hpp"

#include <exception>

#define MAX_POLL 10

class SocketObserver {

  private:
	int fd;

  public:
	SocketObserver();
	~SocketObserver();

	void subscribe(int fd, Socket& observer);
	void poll();
	void unsubscribe(int fd);

	class EpollCreateException : public std::exception {
	  public:
		virtual const char* what() const throw();
	};

	class EpollWaitException : public std::exception {
	  public:
		virtual const char* what() const throw();
	};

	class EpollCtlAddException : public std::exception {
	  public:
		virtual const char* what() const throw();
	};

	class EpollCtlDelException : public std::exception {
	  public:
		virtual const char* what() const throw();
	};
};
