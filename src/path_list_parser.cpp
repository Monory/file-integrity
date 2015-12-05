// Copyright 2015 Nikita Chudinov

#include "path_list_parser.h"
#include <regex>
#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

PathListParser::PathListParser(std::string config_filename) {
    using boost::property_tree::ptree;
    ptree pt;

    read_json(config_filename, pt);
    auto paths = pt.equal_range("");
    for (auto paths_iter = paths.first; paths_iter != paths.second; ++paths_iter) {
        Path path;
        path.path = paths_iter->second.get<std::string>("path");
        path.regex = paths_iter->second.get<std::string>("regex");
        path.recursive = paths_iter->second.get<bool>("recursive");

        this->paths.push_back(path);
    }
}

bool Path::CheckRegex(std::string filename) {
    std::regex expression(regex);
    return std::regex_match(filename, expression);
}

boost::unordered_set<boost::filesystem::path> Path::Files() {
    namespace fs = boost::filesystem;
    boost::unordered_set<fs::path> result;

    fs::path p(path);
    if (recursive) {
        fs::recursive_directory_iterator dir(p), end;

        for (; dir != end; ++dir) {
            fs::path file_path = fs::canonical(dir->path());

            if (fs::is_regular(file_path) && CheckRegex(file_path.filename().string())) {
                result.insert(file_path);
            }
        }
    } else {
        fs::directory_iterator dir(p), end;

        for (; dir != end; ++dir) {
            fs::path file_path = fs::canonical(dir->path());

            if (fs::is_regular(file_path) && CheckRegex(file_path.filename().string())) {
                result.insert(file_path);
            }
        }
    }

    return result;
}

boost::unordered_set<boost::filesystem::path> PathListParser::Files() {
    namespace fs = boost::filesystem;
    boost::unordered_set<fs::path> result;

    for (auto path : paths) {
        auto path_files = path.Files();
        result.insert(path_files.begin(), path_files.end());
    }

    return result;
}
