#include "Command.hpp"

#include <sstream>
#include <iterator>
#include <vector>

void	Command::splitArgs(std::string args, std::vector<std::string>* vstr) {
	std::stringstream	stringStream(args);
	std::istream_iterator<std::string> begin(stringStream);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vstrings(begin, end);
	*vstr = vstrings;
}

const char* Command::MissingParamsInCommandException::what() const throw() {
	return ":Not enough parameters";
	(void)client;
	(void)command;
}

const char* Command::NoChannelException::what() const throw() {
	return ":No such channel";
	(void)client;
	(void)channel;
}

const char* Command::NotOnChannelException::what() const throw() {
	return ":You're not on that channel";
	(void)client;
	(void)channel;
}


const char* Command::UserOnChannelException::what() const throw() {
	return ":is already on channel";
	(void)client;
	(void)nick;
	(void)channel;
}
