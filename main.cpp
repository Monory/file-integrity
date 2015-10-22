#include <string>
#include <getopt.h>
#include <openssl/evp.h>
#include <iostream>
#include <iomanip>
#include <fstream>


void digest_file(std::string filename, unsigned char* digest) {
    std::ifstream file;
    file.open(filename, std::ios::in | std::ios::binary);

    const unsigned int BUFFER_SIZE = 4096;
    char *buffer = new char[BUFFER_SIZE];

    EVP_MD_CTX *context = EVP_MD_CTX_create();
    EVP_MD_CTX_init(context);
    EVP_DigestInit(context, EVP_sha256());


    while (!file.eof()) {
        file.read(buffer, BUFFER_SIZE);
        EVP_DigestUpdate(context, buffer, file.gcount());
    }

    unsigned int digest_size;
    EVP_DigestFinal(context, digest, &digest_size);

    EVP_MD_CTX_cleanup(context);
    EVP_MD_CTX_destroy(context);
}




int main(int argc, char *argv[]) {
    std::string filename;
    int opt;

    while ((opt = getopt(argc, argv, "f:")) != -1) {
        switch (opt) {
            case 'f':
                filename = optarg;
                break;
        }
    }

    

    unsigned char *digest = new unsigned char[32];
    digest_file(filename, digest);

    std::cout << std::hex;
    for (int i = 0; i < 32; ++i) {
        std::cout << std::setfill('0') << std::setw(2) << static_cast<int>(digest[i]);
    }

    return 0;
}

