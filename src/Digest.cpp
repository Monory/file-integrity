// Copyright 2015 Nikita Chudinov

#include "Digest.h"
#include <openssl/evp.h>
#include <string>
#include <fstream>
#include <iostream>

Digest::Digest() {
  context = EVP_MD_CTX_create();
  EVP_MD_CTX_init(context);
}

Digest::~Digest() {
  EVP_MD_CTX_cleanup(context);
  EVP_MD_CTX_destroy(context);
}

void Digest::DigestFile(std::string filename, unsigned char *const digest) const {
  std::ifstream file;
  file.open(filename, std::ios::in | std::ios::binary);

  char *const buffer = new char[BUFFER_SIZE];

  EVP_DigestInit(context, EVP_sha256());


  while (!file.eof()) {
    file.read(buffer, BUFFER_SIZE);
    EVP_DigestUpdate(context, buffer, static_cast<size_t>(file.gcount()));
  }

  uint32_t digest_size;
  EVP_DigestFinal(context, digest, &digest_size);
}
