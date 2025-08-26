#include <charconv>
#include <netinet/in.h>
#include <sys/socket.h>
#include <iostream>
#define PORT 80

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1) {
        std::cout << "Socket creation error!";
        std::cout << errno;;
        return 1;
    }
    struct sockaddr_in address = {
        AF_INET,
        PORT,
        0 //localhost
    };

    //bind 
    int result = bind(sockfd, (struct sockaddr *)&address, sizeof(address));
    if(result == 0) { 
        std::cout << "Binding IP to socket file descriptor";
        std::cout << errno;;
        return 1;
    }
    //listen
    result = listen(sockfd, 10);
    if(result == -1) {
        std::cout << "Setting listen() on socket file descriptor failed";
        std::cout << errno;;
        return 1;
    }
    
    //Последните два аргумента могат да върнат адрес, ако искаме адресът на клиента, 
    //но ние сме използваме localhost за това поставяме стойност 0 и на двете. Ако изпълнението е 
    //успешно, accept() ще върне клиентския файлов дескриптор;
    int clientfd = accept(sockfd, 0 , 0);
    if(sockfd == -1) {
        std::cout << "Socket creation error!";
        std::cout << errno;;
        return 1;
    }
    //
    //The server accepts the connection: 
    //poll() System call for I/O on console
    struct pollfd fds[2] =  {
        {
            0,
            POLLIN,
            0
        },
        {
            clientfd,
            POLLIN,
            0
        }
    };
    //acceput
    return 0;
}
