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

bool Storage::CheckUnits(std::vector<ConfigUnit> units) {
    bool fail = false;

    for (auto unit : units) {
        if (CheckUnit(unit)) {
            fail = true;
        }
    }

    return fail;
}

bool Storage::CheckUnit(ConfigUnit unit) {
    bool fail = false;
    namespace fs = boost::filesystem;
    boost::unordered_set<fs::path> files = unit.Files();

    for (auto pathname : files) {
        auto result = this->CheckDigest(pathname.string());

        switch (result) {
            case Storage::NOT_FOUND:
                fail = true;
                std::cerr << "No hash for this file: " << pathname.string() << std::endl;
                break;
            case Storage::FAIL:
                fail = true;
                std::cerr << "File is corrupted: " << pathname.string() << std::endl;
                break;
            case Storage::PASS:
                break;
        }
    }

    return fail;
}
