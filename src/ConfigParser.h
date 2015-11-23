// Copyright 2015 Nikita Chudinov
#ifndef SRC_CONFIGPARSER_H_
#define SRC_CONFIGPARSER_H_

#include <vector>
#include <string>

struct ParseUnit {
    std::vector<std::string> paths;
    std::vector<std::string> regex;
};

class ConfigParser {
private:
public:
    static std::vector<ParseUnit> ParseConfig(std::string config_filename);
};

#endif  // SRC_CONFIGPARSER_H_
