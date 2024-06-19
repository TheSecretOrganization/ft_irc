#include "Configuration.hpp"

#include <cstddef>

Configuration::Configuration(): userlen(12) {}

Configuration::~Configuration() {}

size_t Configuration::getUserlen() const { return userlen; }
