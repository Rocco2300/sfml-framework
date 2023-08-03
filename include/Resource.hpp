#pragma once

#include <string>

class Resource {
public:
    virtual bool loadFromFile(const std::string& filename) = 0;
};