// Copyright 2015 Nikita Chudinov
#ifndef SRC_DATABASE_H_
#define SRC_DATABASE_H_

#include <db_cxx.h>
#include <string>

// Is POD, so can be written to DB
struct DbData {
    unsigned char digest[32];
    ino_t inode;
    mode_t mode;
    nlink_t hard_links_count;
    uid_t uid;
    gid_t gid;
    off_t byte_size;
    blkcnt_t block_count;
    struct timespec modification_time;
    struct timespec status_change_time;
};


class DbRecord {
public:
    DbRecord();
    DbRecord(std::string filename, struct stat attributes);
    bool operator==(const DbRecord &b);
    bool operator!=(const DbRecord &b);
    std::string filename;
    DbData data;
};


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
