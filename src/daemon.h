// Copyright 2015 Nikita Chudinov
#ifndef SRC_DAEMON_H_
#define SRC_DAEMON_H_

#include "ipc.h"
#include "argument_parser.h"
#include "storage.h"

class Daemon {
public:
    Daemon(ArgumentParser args);
    static void Kill();
private:
    void Store(Storage &storage, std::string path_list_file);
    void Check(Storage &storage, std::string path_list_file);
    void Schedule(Storage &storage, std::string path_list_file, int sleep_duration);

    bool running = true;
};

#endif //SRC_DAEMON_H_
