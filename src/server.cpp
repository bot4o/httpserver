#include "server.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <ostream>

//Constructor
Server::Server() {
    if(CreateServerSocket() != true) {
        std::cout << "ERROR: Server socket failed to create" << std::endl;
        std::cout << "Exiting..." << std::endl;
        return;
    }
}

//Destructor
Server::~Server() {
    int result = close(sockfd);
    if(result == -1) {
        std::cout << "ERROR: Couldn't close server socket";
        return;
    }
    std::cout << "Server closed";
}

bool Server::CreateServerSocket() {
    //The actual server socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1) {
        std::cout << "Socket creation error!" << std::endl;
        std::cout << errno << std::endl;
        return false;
    }

    //Defines a IPv4 domain socket
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;

    //Binds the sockaddr_in and socket
    if(bind(sockfd, (struct sockaddr *)&address, sizeof(address)) == -1) { 
        std::cout << "ERROR: Binding IP to socket file descriptor" << std::endl;
        std::cout << errno << std::endl;
        return false;
    }

    //Prepares the socket for listening
    if(listen(sockfd, 1) == -1) {
        std::cout << "ERROR: Setting listen() on socket file descriptor failed" << std::endl;
        std::cout << errno << std::endl;
        return false;
    }

    return true;
}

bool Server::Start() {
    std::cout << "Server listening on port " << PORT << "...\n";

    //Opens a "session"
    for(;;) {
        //waits for a connection on server socket
        //sets the creates a new clinet socket
        clientfd = accept(sockfd, 0 , 0);
        if(clientfd == -1)  {
            std::cout << "ERROR: Socket creation error!" << std::endl;
            std::cout << errno << std::endl;
            return false;
        }
        char requestBuff[4096];
        ReadClient(requestBuff, sizeof(requestBuff));

        std::cout << requestBuff << std::endl;

        AnalyzeRequest(requestBuff);
    }
}

bool Server::ReadClient(char* requestBuff, int requestBuffSize) {
    //Reads from the clinet AND stores them to buffer 
    int valread = read(clientfd, requestBuff, requestBuffSize);
    if(valread > 0) {
        requestBuff[valread] = '\0';
        return true;
    }
    return false;
}

bool Server::AnalyzeRequest(char* requestBuff) {
    char* method = strtok(requestBuff, " ");
    char* requestTarget = strtok(NULL, " ");
    char* protocol = strtok(NULL, "\r\n");

    if(strcmp(protocol, "HTTP/1.1") == 0) {
        if(strcmp(method, "GET") == 0) {
            if(strcmp(requestTarget, "/") == 0) {
                char path[] = "./public/index.html";
                char fileType[] = "text/html";
                ReturnFile(path, fileType);
                return true;
            }
            else if(strcmp(requestTarget, "/assets/css/style.css") == 0) {
                char path[] = "./public/assets/css/style.css";
                char fileType[] = "text/css";
                ReturnFile(path, fileType);
                return true;
            }
            else if(strcmp(requestTarget, "/favicon.ico") == 0) {
                char path[] = "./public/favicon.ico";
                char fileType[] = "image/x-icon";
                ReturnFile(path, fileType);
                return true;
            }
        }
    }

    return true;
}
bool Server::ReturnFile(char* path, char* type) {
    FILE* file = fopen(path, "r"); //read
    if (!file) {
        const char *notFound =
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/plain\r\n\r\n"
            "404 Not Found";
        SendResponse(notFound);
        return false;
    }

    char fileBuffer[2048];
    long bytesRead = fread(fileBuffer, sizeof(char), sizeof(fileBuffer) - 1 /* space for \0 */, file);
    fileBuffer[bytesRead] = '\0';

    char header[2048];
    snprintf(header, sizeof(header),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: %s\r\n\r\n",
             type);

    char* responseBuff = strcat(header, fileBuffer);
    std::cout << responseBuff << std::endl;
    SendResponse(responseBuff);

    fclose(file);
    return true;
}

bool Server::SendResponse(const char* responseBuff) {
    int result = send(clientfd, responseBuff, strlen(responseBuff), 0);
    if(result == -1) {
        std::cout << "ERROR: Failed to send a response" << std::endl;
        return false;
    }
    else {
        result = close(clientfd);
        if(result == -1) {
            std::cout << "ERROR: Couldn't close client socket";
            return false;
        }
    }
    return true;
}

