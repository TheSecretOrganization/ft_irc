#pragma once

class SocketListener {

  public:
	virtual void onPoll() = 0;
};
