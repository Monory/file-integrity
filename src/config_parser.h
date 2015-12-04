// Copyright 2015 Nikita Chudinov
#ifndef SRC_CONFIG_PARSER_H_
#define SRC_CONFIG_PARSER_H_

#include <vector>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/unordered_set.hpp>

class ConfigUnit {
public:
    boost::unordered_set<boost::filesystem::path> Files();
    std::vector<std::string> paths;
    std::vector<std::string> regex;
private:
    bool CheckRegex(std::string filename);
};


class ConfigParser {
public:
    static std::vector<ConfigUnit> ParseConfig(std::string config_filename);
private:
};

#endif  // SRC_CONFIG_PARSER_H_
