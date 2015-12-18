// Copyright 2015 Nikita Chudinov

#include <iostream>
#include "argument_parser.h"
#include "daemon.h"
#include "ipc.h"

int main(int argc, char *argv[]) {
    ArgumentParser arguments(argc, argv);
    int status = 0;

    switch (arguments.GetMode()) {
        case ArgumentParser::START:
            Daemon::Start();
            break;
        case ArgumentParser::KILL:
            Daemon::Kill();
            break;
        case ArgumentParser::STORE:
        case ArgumentParser::CHECK: {
            IpcConnection socket("\0INTEGRITY");
            IpcClient *client = socket.MakeClient();

            client->SendCommand(arguments.GetMode());
            client->SendString(arguments.GetPathListFile());
            break;
        }
        case ArgumentParser::UNKNOWN:
            status = 1;
        case ArgumentParser::HELP:
        default:
            arguments.PrintHelpMessage();
            break;
    }

    return status;
}
