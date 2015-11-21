// Copyright 2015 Nikita Chudinov
#ifndef SRC_ARGPARSER_H_
#define SRC_ARGPARSER_H_

#include <boost/program_options.hpp>
#include <string>

class ArgParser {
 private:
  enum Mode: int;
  boost::program_options::variables_map vm;
  ArgParser::Mode mode = UNKNOWN;
  static void conflictingOptions(const boost::program_options::variables_map &vm,
                                 const std::string &opt1, const std::string &opt2);
 public:
  enum Mode: int { CHECK = 0, STORE = 1, UNKNOWN = 2 };
  std::string GetConfigFile();
  std::string GetCheckFile();
  ArgParser(int argc, const char *const argv[]);
  Mode GetMode();
};


#endif  // SRC_ARGPARSER_H_
