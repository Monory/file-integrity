// Copyright 2015 Nikita Chudinov

#include <iostream>
#include "argument_parser.h"
#include "config_parser.h"
#include "ipc.h"
#include "storage.h"

int main() {
    IpcConnection conn("\0INTEGRITY");
    conn.Listen();

    while (true) {
        IpcClient *client = conn.WaitForClient();
        int message = client->ReceiveCommand();
        delete client;

        switch (message) {
        case ArgParser::STORE: {
            auto units = ConfigParser::ParseConfig("config.json");
            Storage storage;
            storage.StoreUnits(units);
            break;
        }
        case ArgParser::CHECK: {
            auto units = ConfigParser::ParseConfig("config.json");
            Storage storage;
            storage.CheckUnits(units);
            break;
        }
        default:
            return 1;
    }
    }
}
