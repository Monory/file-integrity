// Copyright 2015 Nikita Chudinov

#include <string>
#include "storage.h"
#include "argument_parser.h"
#include "config_parser.h"

int main(int argc, const char *const argv[]) {
    ArgParser arguments(argc, argv);

    switch (arguments.GetMode()) {
        case ArgParser::STORE: {
            auto units = ConfigParser::ParseConfig(arguments.GetConfigFile());
            Storage storage;
            storage.StoreUnits(units);
            break;
        }
        case ArgParser::CHECK: {
            auto units = ConfigParser::ParseConfig(arguments.GetConfigFile());
            Storage storage;

            return storage.CheckUnits(units);
        }
        default:
            return 1;
    }

    return 0;
}
