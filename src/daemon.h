// Copyright 2015 Nikita Chudinov
#ifndef SRC_DAEMON_H_
#define SRC_DAEMON_H_

#include "ipc.h"

class Daemon {
public:
    static void Start();
    static void Run(IpcConnection conn);
    static void Kill();
};

#endif //SRC_DAEMON_H_
