#include "../include/server.h"
#include <iostream>

Server::Server() {
    CreateServerSocket();
}

Server::~Server() {
    int result = close(sockfd);
    if(result == -1) {
        std::cout << "ERROR: Couldn't close server socket";
        return;
    }

    std::cout << "Server closed";
}

bool Server::CreateServerSocket() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1) {
        std::cout << "Socket creation error!" << std::endl;
        std::cout << errno << std::endl;
        return false;
    }
    struct sockaddr_in address = {
        AF_INET,
        htons(PORT),
        {0} //localhost
    };

    int result = bind(sockfd, (struct sockaddr *)&address, sizeof(address));
    if(result == -1) { 
        std::cout << "ERROR: Binding IP to socket file descriptor" << std::endl;
        std::cout << errno << std::endl;
        return false;
    }

    //Setting server socket to wait for a request from client
    result = listen(sockfd, 1);
    if(result == -1) {
        std::cout << "ERROR: Setting listen() on socket file descriptor failed" << std::endl;
        std::cout << errno << std::endl;
        return false;
    }
    return true;
}

bool Server::Start() {
    std::cout << "Server listening on port " << PORT << "...\n";

    for(;;) {
        clientfd = accept(sockfd, 0 , 0);
        if(clientfd == -1)  {
            std::cout << "ERROR: Socket creation error!" << std::endl;
            std::cout << errno << std::endl;
            return false;
        }

        char* requestBuff = GetRequest();
        std::cout << requestBuff << std::endl;
        AnalyzeRequest(requestBuff);
    }
}

char* Server::GetRequest() {
    char requestBuff[4096];
    int valread = read(clientfd, &requestBuff, sizeof(requestBuff));
    if(valread > 0) {
        requestBuff[valread] = '\0';
        return requestBuff;
    }
    return nullptr;
}

bool Server::AnalyzeRequest(char* requestBuff) {
    char* method = strtok(requestBuff, " ");
    char* requestTarget = strtok(NULL, " ");
    char* protocol = strtok(NULL, "\r\n");

    if(strcmp(protocol, "HTTP/1.1") == 0) {
        if(strcmp(method, "GET") == 0) {
            if(strcmp(requestTarget, "/")) {
                //ReturnFile("./public/index.html", "text/html");
                ReturnIndex();
            }
            if(strcmp(requestTarget, "/assets/css/style.css")) {
                //ReturnFile("./public/index.html", "text/html");
            }
            if(strcmp(requestTarget, "/favicon.ico")) {
                //ReturnFile("./public/index.html", "text/html");
            }
        }
    }

    return true;
}

bool Server::ReturnIndex() {
    FILE* file = fopen("./public/index.html", "r");
    if(!file) {
        std::cout << "ERROR: Could not open index.html\n";
        return false;
    }
    char fileBuffer[2048];
    long bytesRead = fread(fileBuffer, 1, sizeof(fileBuffer) - 1, file);
    fileBuffer[bytesRead] = '\0';
    fclose(file);

    char responseBuff[4096];
    responseBuff[0] = '\0';
    strcat(responseBuff, "HTTP/1.1 200 OK\r\n");
    strcat(responseBuff, "Content-Type: text/html\r\n");
    strcat(responseBuff, "\r\n");
    strcat(responseBuff, fileBuffer);



    SendResponse(responseBuff);
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
