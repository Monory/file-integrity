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

        switch (message) {
            case ArgumentParser::STORE: {
                std::string path_list_file = client->ReceiveString();
                auto path_list = PathListParser(path_list_file);
                Storage storage;
                storage.StorePathListMetadata(path_list);
                break;
            }
            case ArgumentParser::CHECK: {
                std::string path_list_file = client->ReceiveString();
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

        delete client;
    }
}

void Daemon::Kill() {
    IpcConnection socket("\0INTEGRITY");
    IpcClient *client = socket.MakeClient();

    client->SendCommand(ArgumentParser::KILL);
}