// Copyright 2015 Nikita Chudinov

#include "argument_parser.h"
#include <iostream>
#include <string>

ArgParser::ArgParser(int argc, const char *const argv[]) {
    namespace po = boost::program_options;
    po::options_description general_options("General options");

    std::string config;

    general_options.add_options()
        ("help,h", "produce this help message")
        ("store,s", "save file hashes")
        ("check,c", "check file hashes")
        ("config", po::value<std::string>(&config)->default_value("config.json"), "config file location");

    try {
        po::store(po::command_line_parser(argc, argv).options(general_options).run(), vm);
        ArgParser::conflictingOptions(vm, "store", "check");
        po::notify(vm);
        if (vm.count("store")) {
            mode = STORE;
        } else if (vm.count("check")) {
            mode = CHECK;
        } else {
            std::cout << general_options;
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl << general_options;
    }
}

void ArgParser::conflictingOptions(const boost::program_options::variables_map &vm, const std::string &opt1,
                                   const std::string &opt2) {
    if (vm.count(opt1) && !vm[opt1].defaulted() &&
        vm.count(opt2) && !vm[opt2].defaulted()) {
        throw std::logic_error(std::string("Conflicting options '") + opt1 + "' and '" + opt2 + "'.");
    }
}

std::string ArgParser::GetConfigFile() {
    return vm["config"].as<std::string>();
}

ArgParser::Mode ArgParser::GetMode() {
    return mode;
}
