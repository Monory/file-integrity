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
  -c, --check           check all files metadata (or one file, if provided)
  --start               start daemon
  --kill                kills daemon

Start options:
  --config=FILE         starts daemon with provided config (default: config.json))";

    const char *option_string = "sc::h?";
    int index;
    struct option long_options[] = {
        {"help", no_argument, NULL, 'h'},
        {"store", no_argument, NULL, 's'},
        {"check", no_argument, NULL, 'c'},
        {"start", no_argument, NULL, 0},
        {"config", required_argument, NULL, 0},
        {"kill", no_argument, NULL, 0},
        {NULL, 0, 0, 0}
    };

    bool help = false;
    bool store = false;
    bool check = false;
    bool start = false;
    bool kill = false;
    std::string config = "config.json";


    int option = getopt_long(argc, argv, option_string, long_options, &index);
    while (option != -1) {
        switch (option) {
            case 's':
                store = true;
                break;
            case 'c':
                check = true;
                break;
            case 'h':
                help = true;
                break;
            case 0:
                if (strcmp(long_options[index].name, "start") == 0) {
                    start = true;
                } else if (strcmp(long_options[index].name, "kill") == 0) {
                    kill = true;
                } else if (strcmp(long_options[index].name, "config") == 0) {
                    config = optarg;
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

    this->config = config;
}

ArgumentParser::Mode ArgumentParser::GetMode() {
    return mode;
}

void ArgumentParser::PrintHelpMessage() {
    std::cout << HELP_MESSAGE << std::endl;
}

std::string ArgumentParser::GetConfig() {
    return config;
}
