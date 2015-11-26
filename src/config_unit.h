// Copyright 2015 Nikita Chudinov
#ifndef SRC_CONFIG_UNIT_H_
#define SRC_CONFIG_UNIT_H_

#include "boost/filesystem.hpp"
#include "boost/unordered_set.hpp"
#include <vector>
#include <string>
#include <regex>

class ConfigUnit {
public:
    boost::unordered_set<boost::filesystem::path> Files();
    std::vector<std::string> paths;
    std::vector<std::string> regex;
private:
    bool CheckRegex(std::string filename);
};


#endif  // SRC_CONFIG_UNIT_H_
