// Copyright 2015 Nikita Chudinov

#include <boost/filesystem.hpp>
#include "storage.h"
#include <string>
#include <vector>
#include <regex>

void Storage::StoreDigest(std::string filename) {
    unsigned char *data = new unsigned char[DIGEST_SIZE];
    digest.DigestFile(filename, data);
    db.Store(filename, data);
}

Storage::CheckResult Storage::CheckDigest(std::string filename) {
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

bool Storage::CheckRegex(std::string filename, ConfigUnit unit) {
    for (auto regex : unit.regex) {
        std::regex expression(regex);

        if (std::regex_match(filename, expression)) {
            return true;
        }
    }

    return false;
}

void Storage::StoreUnit(ConfigUnit unit) {
    namespace fs = boost::filesystem;
    boost::unordered_set<fs::path> files = unit.Files();

    for (auto pathname : files) {
        std::cout << pathname.string() << std::endl;
        this->StoreDigest(pathname.string());
    }
}

void Storage::StoreUnits(std::vector<ConfigUnit> units) {
    for (auto unit : units) {
        StoreUnit(unit);
    }
}
