// Copyright 2015 Nikita Chudinov

#include "daemon.h"
#include <unistd.h>
#include <iostream>
#include <thread>
#include <chrono>
#include "argument_parser.h"
#include "path_list_parser.h"
#include "ipc.h"
#include "storage.h"
#include "config.h"

Daemon::Daemon(const ArgumentParser &args) {
    ConfigParser config(args.GetConfig());
    plog::init<plog::LogFormatter>(config.GetLogSeverity(), config.GetLogFilename().c_str());

    IpcConnection conn("\0INTEGRITY");
    conn.Listen();

    Storage storage;

    daemon(1, 0);

    std::thread
        schedule(&Daemon::Schedule, this, std::ref(storage), config.GetPathListFile(), config.GetSleepDuration());

    while (running) {
        std::shared_ptr<IpcClient> client = conn.WaitForClient();
        int message = client->ReceiveCommand();

        switch (message) {
            case ArgumentParser::STORE: {
                Store(storage, config.GetPathListFile());
                break;
            }
            case ArgumentParser::CHECK: {
                Check(storage, config.GetPathListFile());
                break;
            }
            case ArgumentParser::KILL:
                running = false;
                storage.mtx.lock(); // wait for all db operations to end
                break;
            default:
                break;
        }
    }

    schedule.join();
}

void Daemon::Kill() {
    IpcConnection socket("\0INTEGRITY");
    std::shared_ptr<IpcClient> client = socket.MakeClient();

    client->SendCommand(ArgumentParser::KILL);
}

void Daemon::Store(Storage &storage, const std::string &path_list_file) {
    auto path_list = PathListParser(path_list_file);
    storage.StorePathListMetadata(path_list);
}

void Daemon::Check(const Storage &storage, const std::string &path_list_file) {
    auto path_list = PathListParser(path_list_file);
    storage.CheckPathListMetadata(path_list);
}

void Daemon::Schedule(Storage &storage, const std::string &path_list_file, int sleep_duration) {
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
