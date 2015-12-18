// Copyright 2015 Nikita Chudinov

#include "argument_parser.h"
#include <cstring>
#include <getopt.h>
#include <unistd.h>
#include <iostream>
#include <string>

ArgumentParser::ArgumentParser(int argc, char *argv[]) {
    HELP_MESSAGE =
R"(General options:
  -h, --help            produce this help message
  -s, --store           scan and store file metadata
  -c, --check[=FILE]    check all files metadata (or one file, if provided)
  --path_list=FILE      use provided config file (default: "./path_list.json")
  --start               start daemon
  --kill                kills daemon)";

    const char *option_string = "sc::h?";
    int index;
    struct option long_options[] = {
        {"help", no_argument, NULL, 'h'},
        {"store", no_argument, NULL, 's'},
        {"check", optional_argument, NULL, 'c'},
        {"path_list", required_argument, NULL, 0},
        {"start", no_argument, NULL, 0},
        {"kill", no_argument, NULL, 0},
        {NULL, 0, 0, 0}
    };

    bool help = false;
    bool store = false;
    bool check = false;
    bool start = false;
    bool kill = false;
    std::string path_list_file = "path_list.json";
    std::string check_file = "";

    int option = getopt_long(argc, argv, option_string, long_options, &index);
    while (option != -1) {
        switch (option) {
            case 's':
                store = true;
                break;
            case 'c':
                check = true;
                if (optarg != NULL) {
                    check_file = optarg;
                }
                break;
            case 'h':
                help = true;
                break;
            case 0:
                if (strcmp(long_options[index].name, "path_list") == 0) {
                    path_list_file = optarg;
                } else if (strcmp(long_options[index].name, "start") == 0) {
                    start = true;
                } else if (strcmp(long_options[index].name, "kill") == 0) {
                    kill = true;
                }
                break;
            default:
                break;
        }
        option = getopt_long(argc, argv, option_string, long_options, &index);
    }


    // fill object
    if (store) {
        mode = STORE;
    }
    if (check) {
        mode = CHECK;
    }
    if (help) {
        mode = HELP;
    }
    if (start) {
        mode = START;
    }
    if (kill) {
        mode = KILL;
    }

    this->path_list_file = path_list_file;
    this->check_file = check_file;
}

std::string ArgumentParser::GetPathListFile() {
    return path_list_file;
}

ArgumentParser::Mode ArgumentParser::GetMode() {
    return mode;
}

void ArgumentParser::PrintHelpMessage() {
    std::cout << HELP_MESSAGE << std::endl;
}
