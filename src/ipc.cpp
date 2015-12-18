// Copyright 2015 Nikita Chudinov

#include "ipc.h"
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <stdexcept>

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
    int status = listen(socket_descriptor, 3);
    if (status != 0) {
        throw std::runtime_error("Socket error. Are your trying to run two instances?");
    }
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

void IpcClient::SendString(std::string message) {
    int size = message.size();
    write(descriptor, &size, sizeof(size));

    //const int BUFFER_SIZE = 128;

    int start = 0;
    while (start < size) {
        int write_size;

        if(size - start > 128) {
            write_size = 128;
        } else {
            write_size = size - start;
        }


        write(descriptor, message.c_str() + start, write_size);
        start += write_size;
    }
}

std::string IpcClient::ReceiveString() {
    int size;
    read(descriptor, &size, sizeof(size));
    std::string result = "";

    const int BUFFER_SIZE = 128;
    char buffer[BUFFER_SIZE];


    while(size > 0) {
        int read_size;

        if (size > BUFFER_SIZE) {
            read_size = BUFFER_SIZE;
        } else {
            read_size = size;
        }

        read(descriptor, buffer, read_size);
        result.append(buffer, read_size);
        size -= read_size;
    }
    return result;
}
