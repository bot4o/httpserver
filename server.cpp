#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>
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
    int clientfd;
    for(;;) {
        //Waiting to get the request 
        //Program wont continue unless something is recived here

        //Последните два аргумента могат да върнат адрес, ако искаме адресът на клиента, 
        //но ние сме използваме localhost за това поставяме стойност 0 и на двете.
        clientfd = accept(sockfd, 0 , 0);
        if(clientfd == -1)  {
            std::cout << "ERROR: Socket creation error!" << std::endl;
            std::cout << errno << std::endl;
            return 1;
        }

        //The request
        char requestBuff[4096] = {0};
        int valread = read(clientfd, &requestBuff, sizeof(requestBuff));
        if(valread > 0) {
            std::cout << "HTTP REQUEST RECIVED" << std::endl;
            std::cout << requestBuff << std::endl;

            //TODO: Process the request
            char* method = strtok(requestBuff, " ");
            char* requestTarget = strtok(NULL, " ");
            char* protocol = strtok(NULL, "\r\n");

            std::cout << "READ:" << method << requestTarget << protocol << "\n";
        }

        //Creating a response
        char responseBuff[4096];
        //Sending the response
        result = send(clientfd, responseBuff, 4096, 0);
        if(result == -1) {
            std::cout << "ERROR: Failed to send a response" << std::endl;
            return 1;
        }
        else {
            std::cout << "Response sent successfully" << std::endl;
            std::cout << result;
        }
    }

    result = close(clientfd);
    if(result == -1) {
        std::cout << "ERROR: Couldn't close client socket";
        return 1;
    }
    result = close(sockfd);
    if(result == -1) {
        std::cout << "ERROR: Couldn't close server socket";
        return 1;
    }

    std::cout << "Server closed";
    return 0;
}
