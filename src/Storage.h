// Copyright 2015 Nikita Chudinov
#ifndef SRC_STORAGE_H_
#define SRC_STORAGE_H_

#include "Database.h"
#include "ConfigParser.h"
#include "Digest.h"
#include <string>

class Storage {
 private:
  Database db;
  Digest digest;
  const uint64_t DIGEST_SIZE = db.DIGEST_SIZE;
  void StoreUnitDigests(ParseUnit unit);
  static bool CheckRegex(std::string path, ParseUnit unit);
 public:
  void StoreDigest(std::string filename);
  int CheckDigest(std::string filename);
  void StoreUnitsDigests(std::vector<ParseUnit> units);
};


#endif  // SRC_STORAGE_H_
