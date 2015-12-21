// Copyright 2015 Nikita Chudinov

#include "storage.h"
#include <sys/stat.h>
#include <string>
#include <vector>
#include <boost/unordered_set.hpp>
#include <boost/filesystem.hpp>
#include <plog/Log.h>

void Storage::StoreMetadata(const std::string &filename) {
    struct stat attributes;
    stat(filename.c_str(), &attributes);

    DbRecord record(filename, attributes);

    digest.DigestFile(filename, record.data.digest);

    db.Store(record);
}

Storage::CheckResult Storage::CheckMetadata(const std::string &filename) const {
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

void Storage::StorePathListMetadata(const PathListParser &parser) {
    namespace fs = boost::filesystem;
    boost::unordered_set<fs::path> files = parser.Files();

    mtx.lock();
    for (auto pathname : files) {
        LOG_INFO << "Scanned " << pathname.string();
        this->StoreMetadata(pathname.string());
    }
    mtx.unlock();
}

bool Storage::CheckPathListMetadata(const PathListParser &parser) const {
    bool fail = false;
    namespace fs = boost::filesystem;

    boost::unordered_set<fs::path> files = parser.Files();

    mtx.lock();
    for (auto pathname : files) {
        auto result = this->CheckMetadata(pathname.string());

        switch (result) {
            case Storage::NOT_FOUND:
                fail = true;
                LOG_WARNING << "No hash for this file: " << pathname.string();
                break;
            case Storage::FAIL:
                fail = true;
                LOG_WARNING << "File is corrupted: " << pathname.string();
                break;
            case Storage::PASS:
                break;
        }
    }
    mtx.unlock();

    return fail;
}
