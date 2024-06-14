#pragma once

class Socket {
  protected:
	int fd;

  public:
	Socket(int fd);
	virtual ~Socket();

	virtual void onPoll() = 0;
	int getFd() const;
};
