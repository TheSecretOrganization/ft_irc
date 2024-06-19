#pragma once

#include <cstddef>

class Configuration {
    private:
        size_t userlen;
    public:
        Configuration();
        ~Configuration();

        size_t getUserlen() const;
};
