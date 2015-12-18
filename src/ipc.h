// Copyright 2015 Nikita Chudinov
#ifndef SRC_SOCKET_H_
#define SRC_SOCKET_H_

#include <string>
#include <sys/socket.h>
#include <sys/un.h>

class IpcClient {
public:
    IpcClient(int client_descriptor);
    IpcClient(int client_descriptor, struct sockaddr_un address);
    ~IpcClient();
    void SendCommand(int message);
    int ReceiveCommand();
    void SendString(std::string message);
    std::string ReceiveString();
private:
    int descriptor;
};


// IPC for InterProcess Communication
class IpcConnection {
public:
    enum Message: int { STORE = 0, CHECK = 1};
    IpcConnection(const char *name);
    ~IpcConnection();
    void Listen();
    IpcClient* WaitForClient();
    IpcClient* MakeClient();
private:
    char* socket_name;
    int socket_descriptor;
    struct sockaddr_un address;
};


#endif  // SRC_SOCKET_H_
