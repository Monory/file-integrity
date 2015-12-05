// Copyright 2015 Nikita Chudinov

#include <iostream>
#include "argument_parser.h"
#include "path_list_parser.h"
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
            auto path_list = PathListParser("path_list.json");
            Storage storage;
            storage.StorePathListMetadata(path_list);
            break;
        }
        case ArgumentParser::CHECK: {
            auto path_list = PathListParser("path_list.json");
            Storage storage;
            storage.CheckPathListMetadata(path_list);
            break;
        }
        default:
            return 1;
    }
    }
}
