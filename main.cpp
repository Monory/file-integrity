#include <string>
#include <getopt.h>
#include <iostream>
#include <iomanip>

int main(int argc, char *argv[]) {
    std::string filename;
    int opt;

    while ((opt = getopt(argc, argv, "f:")) != -1) {
        switch (opt) {
            case 'f':
                filename = optarg;
                break;
        }
    }


    return 0;
}

