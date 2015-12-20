// Copyright 2015 Nikita Chudinov

#include <iostream>
#include "plog/Log.h"
#include "argument_parser.h"
#include "daemon.h"
#include "ipc.h"
#include "logging.h"

int main(int argc, char *argv[]) {
    ArgumentParser arguments(argc, argv);
    int status = 0;

    switch (arguments.GetMode()) {
        case ArgumentParser::START:
            try {
                Daemon d(arguments);
            } catch (std::exception &e) {
                std::cerr << "Error starting daemon: " << e.what() << std::endl;
                status = 1;
            }
            break;
        case ArgumentParser::KILL:
            Daemon::Kill();
            break;
        case ArgumentParser::STORE:
        case ArgumentParser::CHECK: {
            IpcConnection socket("\0INTEGRITY");
            IpcClient *client = socket.MakeClient();

            client->SendCommand(arguments.GetMode());
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
