#include "Command.hpp"

#include <string>
#include <vector>

std::vector<std::string> Command::split(const std::string& str, char del) const {
	std::vector<std::string> result;
	std::string tmp;
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
		if (*it != del) {
			tmp += *it;
		} else if (!tmp.empty()) {
			result.push_back(tmp);
			tmp.clear();
		}
	}
	if (!tmp.empty()) {
		result.push_back(tmp);
	}
	return result;
}
