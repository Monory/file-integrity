// Copyright 2015 Nikita Chudinov

#include "Storage.h"
#include <string>

void Storage::Store(std::string filename) {
  unsigned char *data = new unsigned char[DIGEST_SIZE];
  digest.DigestFile(filename, data);
  db.Store(filename, data);
}

int Storage::Check(std::string filename) {
  unsigned char *data = new unsigned char[DIGEST_SIZE];
  digest.DigestFile(filename, data);

  unsigned char *db_digest = new unsigned char[DIGEST_SIZE];
  if (db.Get(filename, db_digest)) {
    return -1;
  }

  bool identical = true;

  for (int i = 0; i < DIGEST_SIZE; ++i) {
    if (data[i] != db_digest[i]) {
      identical = false;
      break;
    }
  }

  return static_cast<int>(identical);
}
