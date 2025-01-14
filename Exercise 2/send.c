#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1" // IP address of the server
#define SERVER_PORT 12345 // Port number of the server
#define BUF_SIZE 1024 // Buffer size

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char message[BUF_SIZE];
    char buffer[BUF_SIZE];
    socklen_t addr_len = sizeof(server_addr);

    // Create a socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Message input
    printf("Enter a message: ");
    fgets(message, BUF_SIZE, stdin);

    // Remove the newline char from the input
    message[strlen(message) - 1] = '\0';

    // Send the message to the server
    if (sendto(sockfd, message, strlen(message), 0, (struct sockaddr*)&server_addr, addr_len) == -1) {
        perror("Message sending failed");
        exit(EXIT_FAILURE);
    }
    printf("Message sent\n");
}