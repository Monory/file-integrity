// Copyright 2015 Nikita Chudinov
#ifndef SRC_DB_RECORD_H_
#define SRC_DB_RECORD_H_

#include <sys/stat.h>
#include <string>

// Is POD, so can be written to DB
struct DbData {
    char digest[32];
    ino_t inode;
    mode_t mode;
    nlink_t hard_links_count;
    uid_t uid;
    gid_t gid;
    off_t byte_size;
    blkcnt_t block_count;
    struct timespec access_time;
    struct timespec status_change_time;
};

struct DbRecord {
    std::string filename;
    DbData data;
};


#endif  // SRC_DB_RECORD_H_
