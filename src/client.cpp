// Copyright 2015 Nikita Chudinov

#include <iostream>
#include "ipc.h"

int main() {
    IpcConnection socket("\0INTEGRITY");

    IpcClient *client = socket.MakeClient();

    int message;
    std::cin >> message;

    client->SendMessage(message);
    delete client;
}
