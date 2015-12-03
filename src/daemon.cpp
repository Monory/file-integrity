// Copyright 2015 Nikita Chudinov

#include <iostream>
#include "ipc.h"

int main() {
    IpcConnection conn("\0INTEGRITY");
    conn.Listen();

    while (true) {
        IpcClient *client = conn.WaitForClient();
        int message = client->ReceiveMessage();
        delete client;

        if (message == 0) {
            break;
        } else {
            std::cout << message << std::endl;
        }
    }
}
