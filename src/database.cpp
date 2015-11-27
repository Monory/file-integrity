// Copyright 2015 Nikita Chudinov

#include "database.h"
#include <string>
#include <cstring>

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

void Database::Store(std::string filename, unsigned char *digest) {
    char *cstr = new char[filename.length() + 1];
    memcpy(cstr, filename.c_str(), filename.length()+1);

    Dbt key(cstr, static_cast<uint32_t>(filename.length()));
    Dbt value(digest, DIGEST_SIZE);

    db->put(NULL, &key, &value, DB_OVERWRITE_DUP);
}

bool Database::Get(std::string filename, unsigned char *digest) {
    int ret = 0;

    char *cstr = new char[filename.length() + 1];
    memcpy(cstr, filename.c_str(), filename.length()+1);

    Dbt key(cstr, static_cast<uint32_t>(filename.length()));
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

void Database::Store(DbRecord record) {
    // const_cast is acceptable because BDB developers did not properly put const modifiers
    Dbt key(const_cast<char*>(record.filename.c_str()), record.filename.length());
    Dbt value(&record.data, sizeof(record.data));

    db->put(NULL, &key, &value, DB_OVERWRITE_DUP);
}

bool Database::Get(DbRecord *record) {
    int return_value;

    Dbt key(const_cast<char*>(record->filename.c_str()), record->filename.length());
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
