// Copyright 2015 Nikita Chudinov

#include <boost/filesystem.hpp>
#include "Storage.h"
#include <string>
#include <vector>
#include <regex>

void Storage::StoreDigest(std::string filename) {
    unsigned char *data = new unsigned char[DIGEST_SIZE];
    digest.DigestFile(filename, data);
    db.Store(filename, data);
}

int Storage::CheckDigest(std::string filename) {
    namespace fs = boost::filesystem;

    fs::path p(filename);
    p = fs::canonical(p);
    filename = p.string();

    unsigned char *data = new unsigned char[DIGEST_SIZE];
    digest.DigestFile(filename, data);

    unsigned char *db_digest = new unsigned char[DIGEST_SIZE];
    if (db.Get(filename, db_digest)) {
        return NOT_FOUND;
    }

    bool identical = true;

    for (int i = 0; i < DIGEST_SIZE; ++i) {
        if (data[i] != db_digest[i]) {
            identical = false;
            break;
        }
    }

    if (identical) {
        return PASS;
    } else {
        return FAIL;
    }
}

bool Storage::CheckRegex(std::string filename, ParseUnit unit) {
    for (auto regex : unit.regex) {
        std::regex expression(regex);

        if (std::regex_match(filename, expression)) {
            return true;
        }
    }

    return false;
}

void Storage::StoreUnitDigests(ParseUnit unit) {
    namespace fs = boost::filesystem;

    for (auto path : unit.paths) {
        fs::path p(path);

        if (path[path.length() - 1] == '/') {  // If yes, check subdirectories recursively
            fs::recursive_directory_iterator dir(p), end;

            for (; dir != end; ++dir) {
                fs::path filepath = fs::canonical(dir->path());

                if (fs::is_regular(filepath) && CheckRegex(filepath.filename().string(), unit)) {
                    std::cout << filepath.string() << std::endl;
                    this->StoreDigest(filepath.string());
                }
            }
        } else {
            fs::directory_iterator dir(p), end;

            for (; dir != end; ++dir) {
                fs::path filepath = fs::canonical(dir->path());

                if (fs::is_regular(filepath) && CheckRegex(filepath.filename().string(), unit)) {
                    std::cout << filepath.string() << std::endl;
                    this->StoreDigest(filepath.string());
                }
            }
        }
    }
}

void Storage::StoreUnitsDigests(std::vector<ParseUnit> units) {
    for (auto unit : units) {
        StoreUnitDigests(unit);
    }
}
