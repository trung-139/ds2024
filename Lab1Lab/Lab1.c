#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h> // Include unistd.h for close() declaration

int main() {
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    struct hostent *server;

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    // Set socket options
    int optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));

    // Bind the socket to an address
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = 1234;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    // Listen for incoming connections
    listen(sockfd, 5);

    // Accept a connection
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0) {
        perror("ERROR on accept");
        exit(1);
    }

    // Read data from the connection
    bzero(buffer, 256);
    int n = recv(newsockfd, buffer, 255, 0);
    if (n < 0) {
        perror("ERROR reading from socket");
        exit(1);
    }
    printf("Message received: %s\n", buffer);

    // Send data over the connection
    n = send(newsockfd, "Server received your message", 27, 0);
    if (n < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }

    // Close the sockets
    if (close(newsockfd) < 0) { // Check for error in close()
        perror("ERROR closing socket");
        exit(1);
    }
    if (close(sockfd) < 0) { // Check for error in close()
        perror("ERROR closing socket");
        exit(1);
    }

    return 0;
}
