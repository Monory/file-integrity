// Copyright 2015 Nikita Chudinov

#include "ipc.h"
#include <sys/socket.h>
#include <unistd.h>

IpcConnection::IpcConnection(const char *name) {
    socket_descriptor = socket(AF_UNIX, SOCK_STREAM, 0);

    memset(&address, 0, sizeof(address));
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, name, sizeof(address.sun_path)-1);
}

IpcConnection::~IpcConnection() {
    close(socket_descriptor);
    unlink(socket_name);
}

void IpcConnection::Listen() {
    bind(socket_descriptor, (struct sockaddr*)&address, sizeof(address)); // POSIX magic
    listen(socket_descriptor, 3);
}

IpcClient* IpcConnection::WaitForClient() {
    int client = accept(socket_descriptor, NULL, NULL);
    return new IpcClient(client);
}

IpcClient* IpcConnection::MakeClient() {
    return new IpcClient(socket_descriptor, address);
}

IpcClient::IpcClient(int client_descriptor) {
    descriptor = client_descriptor;
}

IpcClient::IpcClient(int client_descriptor, struct sockaddr_un address) : IpcClient(client_descriptor) {
    connect(descriptor, (struct sockaddr*)&address, sizeof(address));
}

IpcClient::~IpcClient() {
    close(descriptor);
}

void IpcClient::SendCommand(int message) {
    write(descriptor, &message, sizeof(message));
}

int IpcClient::ReceiveCommand() {
    int message;
    read(descriptor, &message, sizeof(message));
    return message;
}
