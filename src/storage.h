// Copyright 2015 Nikita Chudinov
#ifndef SRC_STORAGE_H_
#define SRC_STORAGE_H_

#include "database.h"
#include "path_list_parser.h"
#include "digest.h"
#include <string>
#include <vector>

class Storage {
public:
    enum CheckResult: int { PASS = 0, FAIL = 1, NOT_FOUND = 2 };
    void StoreMetadata(std::string filename);
    CheckResult CheckMetadata(std::string filename);
    void StorePathListMetadata(PathListParser parser);
    bool CheckPathListMetadata(PathListParser parser);
private:
    Database db;
    Digest digest;
};


#endif  // SRC_STORAGE_H_
