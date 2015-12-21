// Copyright 2015 Nikita Chudinov
#ifndef SRC_DIGEST_H_
#define SRC_DIGEST_H_

#include <openssl/evp.h>
#include <string>

class Digest {
public:
    Digest();
    ~Digest();
    void DigestFile(const std::string &filename, unsigned char *const digest) const;
private:
    EVP_MD_CTX *context;
    const unsigned int BUFFER_SIZE = 32 * 1024;
};

#endif  // SRC_DIGEST_H_
