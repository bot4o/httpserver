#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>

#define PORT 8080

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1) {
        std::cout << "Socket creation error!" << std::endl;
        std::cout << errno << std::endl;
        return 1;
    }
    struct sockaddr_in address = {
        AF_INET,
        htons(PORT),
        0 //localhost
    };

    int result = bind(sockfd, (struct sockaddr *)&address, sizeof(address));
    if(result == -1) { 
        std::cout << "ERROR: Binding IP to socket file descriptor" << std::endl;
        std::cout << errno << std::endl;
        return 1;
    }

    //Setting server socket to wait for a request from client
    result = listen(sockfd, 1);
    if(result == -1) {
        std::cout << "ERROR: Setting listen() on socket file descriptor failed" << std::endl;
        std::cout << errno << std::endl;
        return 1;
    }
    std::cout << "Server listening on port " << PORT << "...\n";

    //Assgining IP address to the socket: localhost
    //Client socket
    for(;;) {
        //Waiting to get the request 
        //Program wont continue unless something is recived here

        //Последните два аргумента могат да върнат адрес, ако искаме адресът на клиента, 
        //но ние сме използваме localhost за това поставяме стойност 0 и на двете.
        int clientfd = accept(sockfd, 0 , 0);
        if(clientfd == -1)  {
            std::cout << "ERROR: Socket creation error!" << std::endl;
            std::cout << errno << std::endl;
            return 1;
        }

        //The request
        char requestBuff[4096];
        int valread = read(clientfd, &requestBuff, sizeof(requestBuff));
        if(valread > 0) {
            std::cout << requestBuff << std::endl;

            //TODO: Process the request
            char* method = strtok(requestBuff, " ");
            char* requestTarget = strtok(NULL, " ");
            char* protocol = strtok(NULL, "\r\n");

            if(strcmp(protocol, "HTTP/1.1") == 0) {
                if(strcmp(method, "GET") == 0) {
                    if(requestTarget[0] == '/') {
                        FILE* file = fopen("./src/index.html", "r");
                        if(!file) {
                            std::cout << "ERROR: Could not open index.html\n";
                            return 1;
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

                        //SENDING THE RESPONSE
                        result = send(clientfd, responseBuff, strlen(responseBuff), 0);
                        if(result == -1) {
                            std::cout << "ERROR: Failed to send a response" << std::endl;
                            return 1;
                        }
                        else {
                            result = close(clientfd);
                            if(result == -1) {
                                std::cout << "ERROR: Couldn't close client socket";
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }

    result = close(sockfd);
    if(result == -1) {
        std::cout << "ERROR: Couldn't close server socket";
        return 1;
    }

    std::cout << "Server closed";
    return 0;
}
