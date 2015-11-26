// Copyright 2015 Nikita Chudinov
#ifndef SRC_STORAGE_H_
#define SRC_STORAGE_H_

#include "database.h"
#include "config_parser.h"
#include "digest.h"
#include <string>
#include <vector>

class Storage {
public:
    enum CheckResult: int { PASS = 0, FAIL = 1, NOT_FOUND = 2 };
    void StoreDigest(std::string filename);
    CheckResult CheckDigest(std::string filename);
    void StoreUnits(std::vector<ConfigUnit> units);
private:
    Database db;
    Digest digest;
    const uint64_t DIGEST_SIZE = db.DIGEST_SIZE;
    void StoreUnit(ConfigUnit unit);
    static bool CheckRegex(std::string filename, ConfigUnit unit);
};


#endif  // SRC_STORAGE_H_
