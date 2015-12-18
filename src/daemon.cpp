// Copyright 2015 Nikita Chudinov

#include "daemon.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "argument_parser.h"
#include "path_list_parser.h"
#include "ipc.h"
#include "storage.h"

void Daemon::Start(ArgumentParser args) {
    IpcConnection conn("\0INTEGRITY");
    conn.Listen();

    Storage storage;
    std::thread schedule(Daemon::Schedule, std::ref(storage), args.GetPathListFile(), args.GetSleepDuration());

    while (true) {
        IpcClient *client = conn.WaitForClient();
        int message = client->ReceiveCommand();

        switch (message) {
            case ArgumentParser::STORE: {
                Store(storage, args.GetPathListFile());
                break;
            }
            case ArgumentParser::CHECK: {
                Check(storage, args.GetPathListFile());
                break;
            }
            case ArgumentParser::KILL:
                storage.mtx.lock(); // wait for all db operations to end
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

void Daemon::Store(Storage &storage, std::string path_list_file) {
    auto path_list = PathListParser(path_list_file);
    storage.StorePathListMetadata(path_list);
}

void Daemon::Check(Storage &storage, std::string path_list_file) {
    auto path_list = PathListParser(path_list_file);
    storage.CheckPathListMetadata(path_list);
}

void Daemon::Schedule(Storage &storage, std::string path_list_file, int sleep_duration) {
    while (true) {
        Daemon::Check(storage, path_list_file);
        std::this_thread::sleep_for(std::chrono::seconds(sleep_duration));
    }
}
