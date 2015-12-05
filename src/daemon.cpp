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
        case ArgumentParser::STORE: {
            auto config = ConfigParser("config.json");
            Storage storage;
            storage.StoreConfigFiles(config);
            break;
        }
        case ArgumentParser::CHECK: {
            auto config = ConfigParser("config.json");
            Storage storage;
            storage.CheckConfigFiles(config);
            break;
        }
        default:
            return 1;
    }
    }
}
