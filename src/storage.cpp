// Copyright 2015 Nikita Chudinov

#include "storage.h"
#include <sys/stat.h>
#include <string>
#include <vector>
#include <boost/unordered_set.hpp>
#include <boost/filesystem.hpp>

void Storage::StoreMetadata(std::string filename) {
    struct stat attributes;
    stat(filename.c_str(), &attributes);

    DbRecord record(filename, attributes);

    digest.DigestFile(filename, record.data.digest);

    db.Store(record);
}

Storage::CheckResult Storage::CheckMetadata(std::string filename) {
    struct stat attributes;
    stat(filename.c_str(), &attributes);
    DbRecord current_metadata(filename, attributes);

    digest.DigestFile(filename, current_metadata.data.digest);

    DbRecord database_metadata;
    database_metadata.filename = filename;
    if (db.Get(&database_metadata)) {
        return NOT_FOUND;
    }

    if (current_metadata == database_metadata) {
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
        this->StoreMetadata(pathname.string());
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
        auto result = this->CheckMetadata(pathname.string());

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
