// Copyright 2015 Nikita Chudinov

#include <string>
#include "Storage.h"
#include "ArgParser.h"


int main(int argc, const char *const argv[]) {
  ArgParser arguments(argc, argv);

  switch (arguments.GetMode()) {
    case ArgParser::STORE:
      break;
    case ArgParser::CHECK:
      break;
    default:
      return 1;
  }

  return 0;
}
