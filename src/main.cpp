// Copyright 2015 Nikita Chudinov

#include <getopt.h>
#include <db_cxx.h>
#include <iomanip>
#include <fstream>
#include <cstring>
#include "Digest.h"

int parse_args(int argc, char *argv[], std::string *filename, bool *mode) {
  int opt;

  bool store = false;
  bool check = false;

  while ((opt = getopt(argc, argv, "sc")) != -1) {
    switch (opt) {
      case 's':
        store = true;
        break;
      case 'c':
        check = true;
        break;
    }
  }

  if (optind < argc) {
    *filename = argv[optind];
  }

  if (!(check || store)) {
    std::cerr << "choose mode: -s for store or -c for check" << std::endl;
    return 1;
  }

  if (check && store) {
    std::cerr << "choose only one mode: -s for store or -c for check" << std::endl;
    return 1;
  }

  if (filename->length() == 0) {
    std::cerr << "enter filename" << std::endl;
    return 1;
  }

  if ((*filename)[0] != '/') {
    std::cerr << "use absolute paths" << std::endl;
    return 1;
  }

  *mode = store;

  return 0;
}

void db_store(std::string filename, unsigned char *digest) {
  Db db(NULL, 0);

  try {
    db.open(NULL, "integrity.db", NULL, DB_BTREE, DB_CREATE, 0);

    char *cstr = new char[filename.length() + 1];
    std::strcpy(cstr, filename.c_str());
    Dbt key(cstr, filename.length());

    Dbt value(digest, DIGEST_SIZE);

    int ret = db.put(NULL, &key, &value, DB_OVERWRITE_DUP);
    db.close(0);
  } catch (DbException &e) {
    std::cerr << "Error storing data" << std::endl;
  }
}

int db_get(std::string filename, unsigned char *digest) {
  Db db(NULL, 0);
  int ret;

  try {
    db.open(NULL, "integrity.db", NULL, DB_BTREE, DB_CREATE, 0);

    char *cstr = new char[filename.length() + 1];
    std::strcpy(cstr, filename.c_str());
    Dbt key(cstr, filename.length());

    Dbt data;
    data.set_data(digest);
    data.set_ulen(DIGEST_SIZE);
    data.set_flags(DB_DBT_USERMEM);

    ret = db.get(NULL, &key, &data, 0);

    db.close(0);
  } catch (DbException &e) {
    std::cerr << "Error getting data" << std::endl;
  }

  if (ret == DB_NOTFOUND) {
    return 1;
  } else {
    return 0;
  }
}

void store(std::string filename, const Digest &d) {
  unsigned char *digest = new unsigned char[DIGEST_SIZE];
  d.DigestFile(filename, digest);

  db_store(filename, digest);
  std::cout << filename << ": ";

  std::cout << std::hex;
  for (int i = 0; i < DIGEST_SIZE; ++i) {
    std::cout << std::setfill('0') << std::setw(2) << static_cast<int>(digest[i]);
  }
  std::cout << std::endl << std::dec;
}

void check(std::string filename, const Digest &d) {
  unsigned char *digest = new unsigned char[DIGEST_SIZE];
  d.DigestFile(filename, digest);

  unsigned char *db_digest = new unsigned char[DIGEST_SIZE];
  if (db_get(filename, db_digest)) {
    std::cout << "No such file in database" << std::endl;
    return;
  }

  bool identical = true;

  for (int i = 0; i < DIGEST_SIZE; ++i) {
    if (digest[i] != db_digest[i]) {
      identical = false;
      break;
    }
  }

  if (identical) {
    std::cout << "File is correct" << std::endl;
  } else {
    std::cout << "File is corrupted" << std::endl;
  }
}


int main(int argc, char *argv[]) {
  std::string filename;
  bool mode;

  if (parse_args(argc, argv, &filename, &mode)) {
    return 1;
  }

  Digest d;


  if (mode) {
    store(filename, d);
  } else {
    check(filename, d);
  }

  return 0;
}
