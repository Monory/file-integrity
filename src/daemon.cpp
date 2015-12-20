// Copyright 2015 Nikita Chudinov

#include "daemon.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "argument_parser.h"
#include "path_list_parser.h"
#include "ipc.h"
#include "storage.h"

Daemon::Daemon(ArgumentParser args) {
    IpcConnection conn("\0INTEGRITY");
    conn.Listen();

    Storage storage;
    std::thread schedule(&Daemon::Schedule, this, std::ref(storage), args.GetPathListFile(), args.GetSleepDuration());

    while (running) {
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
                running = false;
                storage.mtx.lock(); // wait for all db operations to end
                break;
            default:
                break;
        }

        delete client;
    }

    schedule.join();
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
    int time_left = sleep_duration;

    while (running) {
        if (time_left) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            --time_left;
        } else {
            Daemon::Check(storage, path_list_file);
            time_left = sleep_duration;
        }
    }
}
