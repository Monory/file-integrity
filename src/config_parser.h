// Copyright 2015 Nikita Chudinov
#ifndef SRC_CONFIG_PARSER_H_
#define SRC_CONFIG_PARSER_H_

#include <vector>
#include <string>

struct ConfigUnit {
    std::vector<std::string> paths;
    std::vector<std::string> regex;
};

class ConfigParser {
public:
    static std::vector<ConfigUnit> ParseConfig(std::string config_filename);
private:
};

#endif  // SRC_CONFIG_PARSER_H_
