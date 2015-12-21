// Copyright 2015 Nikita Chudinov
#ifndef SRC_STORAGE_H_
#define SRC_STORAGE_H_

#include "database.h"
#include "path_list_parser.h"
#include "digest.h"
#include <string>
#include <mutex>
#include <vector>

class Storage {
public:
    enum CheckResult: int { PASS = 0, FAIL = 1, NOT_FOUND = 2 };
    void StorePathListMetadata(const PathListParser &parser);
    bool CheckPathListMetadata(const PathListParser &parser) const;
    mutable std::mutex mtx;
private:
    void StoreMetadata(const std::string &filename);
    CheckResult CheckMetadata(const std::string &filename) const;
    Database db;
    Digest digest;
};


#endif  // SRC_STORAGE_H_
