// Copyright 2015 Nikita Chudinov
#ifndef SRC_DATABASE_H_
#define SRC_DATABASE_H_

#include <db_cxx.h>
#include <string>

class Database {
public:
    static const uint64_t DIGEST_SIZE = 32;
    Database();
    ~Database();
    void Store(std::string filename, unsigned char *digest);
    int Get(std::string filename, unsigned char *digest);
private:
    Db *db = new Db(NULL, 0);
};


#endif  // SRC_DATABASE_H_
