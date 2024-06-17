#pragma once

#include "Socket.hpp"

#include <exception>
#include <string>

class ClientSocket : public Socket {
  private:
	void onPoll();

  public:
	ClientSocket(int fd);
	~ClientSocket();

	void sendPacket(std::string packet) const;

	class SendException : public std::exception {
	  public:
		virtual const char* what() const throw();
	};
};
