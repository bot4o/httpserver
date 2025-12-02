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
    //Sockfd variable 
    int sockfd;
    char requestBuff[4096];
    int clientfd;

    //Creates the socket for the server
    bool CreateServerSocket();
    //Gets content from clinet 
    bool ReadClient(char* requestBuff);
    //logic for analyzsingHTTP request
    bool AnalyzeRequest(char* requestBuff);

    bool ReturnFile(char* path, char *type);

    bool SendResponse(const char* responseBuff);
public:
    //Constructor
    Server();
    //Destructor
    virtual ~Server();

    bool Start();
};
