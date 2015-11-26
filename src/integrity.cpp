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
            storage.StoreUnitsDigests(units);
            break;
        }
        case ArgParser::CHECK: {
            Storage storage;
            int result = storage.CheckDigest(arguments.GetCheckFile());

            switch (result) {
                case Storage::NOT_FOUND:
                    std::cerr << "No hash for this file" << std::endl;
                    break;
                case Storage::FAIL:
                    std::cerr << "File is corrupted" << std::endl;
                    break;
                case Storage::PASS:
                    std::cout << "File is valid" << std::endl;
            }

            break;
        }
        default:
            return 1;
    }

    return 0;
}
