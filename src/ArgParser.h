// Copyright 2015 Nikita Chudinov
#ifndef SRC_ARGPARSER_H_
#define SRC_ARGPARSER_H_

#include <boost/program_options.hpp>
#include <string>

class ArgParser {
 private:
  void conflictingOptions(const boost::program_options::variables_map &vm,
                          const std::string &opt1, const std::string &opt2);
  boost::program_options::variables_map vm;
 public:
  enum Mode {STORE, CHECK};
  std::string GetConfigFile();
  ArgParser::Mode GetMode();
  ArgParser(int argc, const char *const argv[]);
};


#endif  // SRC_ARGPARSER_H_
