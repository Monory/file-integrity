// Copyright 2015 Nikita Chudinov
#ifndef SRC_CONFIG_PARSER_H_
#define SRC_CONFIG_PARSER_H_

#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/unordered_set.hpp>

class Path {
public:
    boost::unordered_set<boost::filesystem::path> Files();
    std::string path;
    std::string regex;
    bool recursive;
private:
    bool CheckRegex(std::string filename);
};


class ConfigParser {
public:
    ConfigParser(std::string config_filename);
    boost::unordered_set<boost::filesystem::path> Files();
    //void ReparseConfig();
    //void AddPath(Path path);
    //static std::vector<Path> ParseConfig(std::string config_filename);
private:
    std::vector<Path> paths;
};

#endif  // SRC_CONFIG_PARSER_H_
