// Copyright 2015 Nikita Chudinov
#ifndef SRC_CONFIG_PARSER_H_
#define SRC_CONFIG_PARSER_H_

#include <vector>
#include <string>
#include "config_unit.h"

class ConfigParser {
public:
    static std::vector<ConfigUnit> ParseConfig(std::string config_filename);
private:
};

#endif  // SRC_CONFIG_PARSER_H_
