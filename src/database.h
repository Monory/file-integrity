// Copyright 2015 Nikita Chudinov
#ifndef SRC_DATABASE_H_
#define SRC_DATABASE_H_

#include <db_cxx.h>
#include <string>
#include "db_record.h"

class Database {
public:
    static const uint64_t DIGEST_SIZE = 32;
    Database();
    ~Database();
    void Store(std::string filename, unsigned char *digest);
    bool Get(std::string filename, unsigned char *digest);
    void Store(DbRecord record);
    bool Get(DbRecord *record);
private:
    Db *db = new Db(NULL, 0);
};


#endif  // SRC_DATABASE_H_
