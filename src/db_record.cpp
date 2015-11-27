// Copyright 2015 Nikita Chudinov

#include "db_record.h"

DbRecord::DbRecord() {}

DbRecord::DbRecord(std::string filename, struct stat attributes) {
    this->filename = filename;

    this->data.inode = attributes.st_ino;
    this->data.mode = attributes.st_mode;
    this->data.hard_links_count = attributes.st_nlink;
    this->data.uid = attributes.st_uid;
    this->data.gid = attributes.st_gid;
    this->data.byte_size = attributes.st_size;
    this->data.block_count = attributes.st_blocks;
    this->data.modification_time = attributes.st_mtim;
    this->data.status_change_time = attributes.st_ctim;
}

bool DbRecord::operator==(const DbRecord &b) {
    if(filename != b.filename ||
        data.inode != b.data.inode ||
        data.mode != b.data.mode ||
        data.hard_links_count != b.data.hard_links_count ||
        data.uid != b.data.uid ||
        data.gid != b.data.gid ||
        data.byte_size != b.data.byte_size ||
        data.block_count != b.data.block_count ||
        data.modification_time.tv_sec != b.data.modification_time.tv_sec ||
        data.modification_time.tv_nsec != b.data.modification_time.tv_nsec ||
        data.status_change_time.tv_sec != b.data.status_change_time.tv_sec ||
        data.status_change_time.tv_nsec != b.data.status_change_time.tv_nsec) {
        return false;
    }

    for (int i = 0; i < sizeof(data.digest)/sizeof(data.digest[0]); ++i) {
        if (data.digest[i] != b.data.digest[i]) {
            return false;
        }
    }

    return true;
}

bool DbRecord::operator!=(const DbRecord &b) {
    return !(*this == b);
}
