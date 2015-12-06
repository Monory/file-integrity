// Copyright 2015 Nikita Chudinov

#include "daemon.h"
#include <iostream>
#include "argument_parser.h"
#include "path_list_parser.h"
#include "ipc.h"
#include "storage.h"

void Daemon::Start() {
    IpcConnection conn("\0INTEGRITY");
    conn.Listen();

    while (true) {
        IpcClient *client = conn.WaitForClient();
        int message = client->ReceiveCommand();
        std::string path_list_file = client->ReceiveString();
        delete client;

        switch (message) {
            case ArgumentParser::STORE: {
                auto path_list = PathListParser(path_list_file);
                Storage storage;
                storage.StorePathListMetadata(path_list);
                break;
            }
            case ArgumentParser::CHECK: {
                auto path_list = PathListParser(path_list_file);
                Storage storage;
                storage.CheckPathListMetadata(path_list);
                break;
            }
            case ArgumentParser::KILL:
                exit(0);
            default:
                break;
        }
    }
}
