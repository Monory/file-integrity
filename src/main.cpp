// Copyright 2015 Nikita Chudinov

#include <getopt.h>
#include <string>
#include "Storage.h"

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

int main(int argc, char *argv[]) {
  std::string filename;
  bool mode;

  if (parse_args(argc, argv, &filename, &mode)) {
    return 1;
  }

  Storage s;

  if (mode) {
    s.Store(filename);
  } else {
    std::cout << s.Check(filename);
  }

  return 0;
}
