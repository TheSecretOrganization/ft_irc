#pragma once

class Socket {

  public:
	virtual void onPoll() = 0;
};
