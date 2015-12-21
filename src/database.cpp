// Copyright 2015 Nikita Chudinov

#include "database.h"
#include <sys/stat.h>
#include <cstring>
#include <string>
#include <vector>

DbRecord::DbRecord() { }

DbRecord::DbRecord(const std::string &filename, const struct stat &attributes) {
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

bool DbRecord::operator==(const DbRecord &b) const {
    if (filename != b.filename ||
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

    for (int i = 0; i < sizeof(data.digest) / sizeof(data.digest[0]); ++i) {
        if (data.digest[i] != b.data.digest[i]) {
            return false;
        }
    }

    return true;
}

bool DbRecord::operator!=(const DbRecord &b) const {
    return !(*this == b);
}


Database::Database() {
    try {
        db->open(NULL, "integrity.db", NULL, DB_BTREE, DB_CREATE, 0);
    } catch (DbException &e) {
        std::cerr << "Error opening db" << std::endl;
    }
}

Database::~Database() {
    db->close(0);
}

void Database::Store(const std::string &filename, unsigned char *digest) {
    std::vector<char> cstr(filename.length() + 1);
    memcpy(cstr.data(), filename.c_str(), filename.length() + 1);

    Dbt key(cstr.data(), static_cast<uint32_t>(filename.length()));
    Dbt value(digest, DIGEST_SIZE);

    db->put(NULL, &key, &value, DB_OVERWRITE_DUP);
}

bool Database::Get(const std::string &filename, unsigned char *digest) const {
    int ret = 0;

    std::vector<char> cstr(filename.length() + 1);
    memcpy(cstr.data(), filename.c_str(), filename.length() + 1);

    Dbt key(cstr.data(), static_cast<uint32_t>(filename.length()));
    Dbt data(digest, DIGEST_SIZE);
    data.set_ulen(DIGEST_SIZE);
    data.set_flags(DB_DBT_USERMEM);

    ret = db->get(NULL, &key, &data, 0);

    if (ret == DB_NOTFOUND) {
        return 1;
    } else {
        return 0;
    }
}

void Database::Store(DbRecord &record) {
    // const_cast is acceptable because BDB developers did not properly put const modifiers
    Dbt key(const_cast<char *>(record.filename.c_str()), record.filename.length());
    Dbt value(&record.data, sizeof(record.data));

    db->put(NULL, &key, &value, DB_OVERWRITE_DUP);
}

bool Database::Get(DbRecord *record) const {
    int return_value;

    Dbt key(const_cast<char *>(record->filename.c_str()), record->filename.length());
    Dbt data(&record->data, sizeof(record->data));
    data.set_ulen(sizeof(record->data));
    data.set_flags(DB_DBT_USERMEM);

    return_value = db->get(NULL, &key, &data, 0);

    if (return_value == DB_NOTFOUND) {
        return true;
    } else {
        return false;
    }
}
