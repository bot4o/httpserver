#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>
#include <string>
#define PORT 8080

int main() {
    std::string url = "http://localhost:" + std::to_string(PORT);
    //Server socket
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

    //Assgining IP address to the socket: localhost
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
    //Последните два аргумента могат да върнат адрес, ако искаме адресът на клиента, 
    //но ние сме използваме localhost за това поставяме стойност 0 и на двете. Ако изпълнението е 
    //успешно, accept() ще върне клиентския файлов дескриптор;

    //Client socket
    int clientfd = -1;
    for(;;) {
        //Waiting to get the request 
        //Program wont continue unless something is recived here
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
        }
        //TODO: Process the request
        
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
        
        std::cout << "--------------------------------------------------------------" << std::endl;
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
