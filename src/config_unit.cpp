// Copyright 2015 Nikita Chudinov

#include "config_unit.h"

boost::unordered_set<boost::filesystem::path> ConfigUnit::Files() {
    namespace fs = boost::filesystem;
    boost::unordered_set<fs::path> result;

    for (auto pathname : paths) {
        fs::path p(pathname);

        // If path ends with forward slash, check subdirectories recursively
        if (pathname[pathname.length() - 1] == '/' && pathname.length() > 1) {
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
    }

    return result;
}

bool ConfigUnit::CheckRegex(std::string filename) {
    for (auto expr : regex) {
        std::regex expression(expr);

        if (std::regex_match(filename, expression)) {
            return true;
        }
    }

    return false;
}
