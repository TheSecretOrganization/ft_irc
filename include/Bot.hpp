#pragma once

#include <string>

class Bot {
  private:
	Client* bot;
  public:
	Bot();
	~Bot();

	const std::string& getNameFromPrefix(const std::string& prefix);
	void kickUser(const std::string& prefix, const std::string& channel, const std::string& word);
	void scanMessage(const std::string& prefix, const std::string& channel, const std::string& message);
};
