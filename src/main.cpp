// Copyright 2015 Nikita Chudinov

#include <getopt.h>
#include <string>
#include "Storage.h"
#include "ArgParser.h"


int main(int argc, const char *const argv[]) {
  ArgParser arguments(argc, argv);

  std::cout << arguments.GetConfigFile() << std::endl;

  return 0;
}
