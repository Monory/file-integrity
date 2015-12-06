// Copyright 2015 Nikita Chudinov

#include <iostream>
#include "argument_parser.h"

int main(int argc, char *argv[]) {
    ArgumentParser arguments(argc, argv);

    switch (arguments.GetMode()) {
        case ArgumentParser::HELP:
            arguments.PrintHelpMessage();
            break;
        default:
            std::cout << "Totally not a help message." << std::endl;
            break;
    }

    return 0;
}
