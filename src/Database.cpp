// Copyright 2015 Nikita Chudinov

#include "Database.h"
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
  std::snprintf(cstr, filename.length() + 1, filename.c_str());

  Dbt key(cstr, static_cast<uint32_t>(filename.length()));
  Dbt value(digest, DIGEST_SIZE);

  db->put(NULL, &key, &value, DB_OVERWRITE_DUP);
}

int Database::Get(std::string filename, unsigned char *digest) {
  int ret = 0;

  char *cstr = new char[filename.length() + 1];
  std::snprintf(cstr, filename.length() + 1, filename.c_str());

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
