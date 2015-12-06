// Copyright 2015 Nikita Chudinov

#include <iostream>
#include "argument_parser.h"
#include "daemon.h"
#include "ipc.h"

int main(int argc, char *argv[]) {
    ArgumentParser arguments(argc, argv);

    switch (arguments.GetMode()) {
        case ArgumentParser::START:
            Daemon::Start();
            break;
        case ArgumentParser::KILL:
            Daemon::Kill();
            break;
        case ArgumentParser::UNKNOWN:
        case ArgumentParser::HELP:
            arguments.PrintHelpMessage();
            break;
        default:
            IpcConnection socket("\0INTEGRITY");
            IpcClient *client = socket.MakeClient();

            client->SendCommand(arguments.GetMode());
            client->SendString(arguments.GetPathListFile());
            break;
    }

    return 0;
}
