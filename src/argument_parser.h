// Copyright 2015 Nikita Chudinov
#ifndef SRC_ARGUMENT_PARSER_H_
#define SRC_ARGUMENT_PARSER_H_

#include <string>

class ArgumentParser {
public:
    enum Mode: int { CHECK = 0, STORE = 1, HELP = 2, UNKNOWN = -1 };
    ArgumentParser(int argc, char *argv[]);
    std::string GetPathListFile();
    Mode GetMode();
private:
    ArgumentParser::Mode mode = UNKNOWN;
    std::string path_list_file = "path_list.json";
    std::string check_file = "";
};

#endif  // SRC_ARGUMENT_PARSER_H_
