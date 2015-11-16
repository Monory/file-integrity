#include <getopt.h>
#include <openssl/evp.h>
#include <db_cxx.h>
#include <iomanip>
#include <fstream>
#include <cstring>


const unsigned int DIGEST_SIZE = 32;

void digest_file(std::string filename, unsigned char *digest) {
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

void store(std::string filename) {
  unsigned char *digest = new unsigned char[DIGEST_SIZE];
  digest_file(filename, digest);

  db_store(filename, digest);
  std::cout << filename << ": ";

  std::cout << std::hex;
  for (int i = 0; i < DIGEST_SIZE; ++i) {
    std::cout << std::setfill('0') << std::setw(2) << static_cast<int>(digest[i]);
  }
  std::cout << std::endl << std::dec;
}

void check(std::string filename) {
  unsigned char *digest = new unsigned char[DIGEST_SIZE];
  digest_file(filename, digest);

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


  if (mode) {
    store(filename);
  } else {
    check(filename);
  }

  return 0;
}
