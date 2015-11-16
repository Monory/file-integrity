// Copyright 2015 Nikita Chudinov
#ifndef SRC_STORAGE_H_
#define SRC_STORAGE_H_

#include "Database.h"
#include "Digest.h"
#include <string>

class Storage {
 private:
  Database db;
  Digest digest;
  const uint64_t DIGEST_SIZE = db.DIGEST_SIZE;
 public:
  void Store(std::string filename);
  int Check(std::string filename);
};


#endif  // SRC_STORAGE_H_
