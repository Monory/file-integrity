// Copyright 2015 Nikita Chudinov

#include <getopt.h>
#include <db_cxx.h>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <string>
#include "Digest.h"
#include "Database.h"

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

void store(std::string filename, const Digest &d, Database *db) {
  const uint64_t DIGEST_SIZE = db->DIGEST_SIZE;

  unsigned char *digest = new unsigned char[DIGEST_SIZE];
  d.DigestFile(filename, digest);

  db->Store(filename, digest);
  std::cout << filename << ": ";

  std::cout << std::hex;
  for (int i = 0; i < DIGEST_SIZE; ++i) {
    std::cout << std::setfill('0') << std::setw(2) << static_cast<int>(digest[i]);
  }
  std::cout << std::endl << std::dec;
}

void check(std::string filename, const Digest &d, Database *db) {
  const uint64_t DIGEST_SIZE = db->DIGEST_SIZE;

  unsigned char *digest = new unsigned char[DIGEST_SIZE];
  d.DigestFile(filename, digest);

  unsigned char *db_digest = new unsigned char[DIGEST_SIZE];
  if (db->Get(filename, db_digest)) {
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

  Digest digest;
  Database db;


  if (mode) {
    store(filename, digest, &db);
  } else {
    check(filename, digest, &db);
  }

  return 0;
}
