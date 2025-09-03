#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>
#include <string>
#define PORT 8080

int main() {
    std::string url = "http://localhost:" + std::to_string(PORT);
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

    //Read HTTP request
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
    
    int clientfd;
    for(;;) {
        clientfd= accept(sockfd, 0 , 0);
        if(clientfd == -1) {
            std::cout << "ERROR: Socket creation error!" << std::endl;
            std::cout << errno << std::endl;
            return 1;
        }
        

        char buff[4096] = {0};
        int valread = read(clientfd, &buff, sizeof(buff));
        if(valread > 0) {
            std::cout << "HTTP REQUEST RECIVED" << std::endl;
            std::cout << buff << std::endl;
        }

        result = send(clientfd, buff, 4096, 0);
        if(result == -1) {
            std::cout << "failed to send buff";
        }
        else {
            std::cout << "send success" << "\n";
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
