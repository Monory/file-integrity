// Copyright 2015 Nikita Chudinov
#ifndef SRC_DATABASE_H_
#define SRC_DATABASE_H_

#include <db_cxx.h>
#include <string>
#include <memory>

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
    bool operator==(const DbRecord &b) const;
    bool operator!=(const DbRecord &b) const;
    std::string filename;
    DbData data;
};


class Database {
public:
    static const uint64_t DIGEST_SIZE = 32;
    Database();
    ~Database();
    void Store(std::string filename, unsigned char *digest);
    bool Get(std::string filename, unsigned char *digest) const;
    void Store(DbRecord record);
    bool Get(DbRecord *record) const;
private:
    std::shared_ptr<Db> db = std::make_shared<Db>(nullptr, 0);
};


#endif  // SRC_DATABASE_H_
