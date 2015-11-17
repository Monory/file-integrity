// Copyright 2015 Nikita Chudinov
#ifndef SRC_DATABASE_H_
#define SRC_DATABASE_H_

#include <db_cxx.h>
#include <string>

class Database {
 private:
  Db *db = new Db(NULL, 0);
 public:
  const uint64_t DIGEST_SIZE = 32;
  Database();
  ~Database();
  void Store(std::string filename, unsigned char *digest);
  int Get(std::string filename, unsigned char *digest);
};


#endif  // SRC_DATABASE_H_
