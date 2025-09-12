#pragma once

#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>

#define PORT 8080
#define VIEWSDIR = "./public/";

class Server {
private:
    int sockfd;
    int clientfd;
    bool CreateServerSocket();
    bool GetRequest(char* requestBuff);
    bool AnalyzeRequest(char* requestBuff);
    bool ReturnFile(char* path, char *type);
    bool SendResponse(const char* responseBuff);
public:
    Server();
    virtual ~Server();
    bool Start();
};
