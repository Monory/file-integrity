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
    bool CheckUnits(std::vector<ConfigUnit> units);
private:
    void StoreUnit(ConfigUnit unit);
    bool CheckUnit(ConfigUnit unit);
    Database db;
    Digest digest;
    const uint64_t DIGEST_SIZE = db.DIGEST_SIZE;
};


#endif  // SRC_STORAGE_H_
