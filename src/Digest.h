// Copyright 2015 Nikita Chudinov
#ifndef DIGEST_H_
#define DIGEST_H_

#include <openssl/evp.h>
#include <string>
#include <fstream>

const uint64_t DIGEST_SIZE = 32;

class Digest {
 private:
  EVP_MD_CTX *context;
  const unsigned int BUFFER_SIZE = 4096;
 public:
  Digest();
  ~Digest();
  void DigestFile(std::string filename, unsigned char *const digest) const;
};

#endif  // DIGEST_H_
