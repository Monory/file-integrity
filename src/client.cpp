// Copyright 2015 Nikita Chudinov

#include <iostream>
#include "ipc.h"
#include "argument_parser.h"

int main(int argc, const char *const argv[]) {
    ArgParser arguments(argc, argv);

    IpcConnection socket("\0INTEGRITY");
    IpcClient *client = socket.MakeClient();

    client->SendCommand(arguments.GetMode());

    return 0;
}
