#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#define PORT 80

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address = {
        AF_INET,
        PORT,
        0
    };

    //bind 
    int result = bind(sockfd, (struct sockaddr *)&address, sizeof(address));
    

    //listen
    
    //accept
    return 0;
}
