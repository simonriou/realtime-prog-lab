#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_ADDR "10.100.23.204"  // Replace with your server's IP address
#define SERVER_PORT 20018        // Port to send the message to
#define MAX_BUFFER_SIZE 1024

int main() {
    int send_socket, recv_socket;
    struct sockaddr_in server_addr, recv_addr;
    char message[] = "Hello, Server!";
    char buffer[MAX_BUFFER_SIZE];
    socklen_t addr_len = sizeof(struct sockaddr_in);

    // Create the sending socket
    send_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (send_socket < 0) {
        perror("Send socket creation failed");
        exit(1);
    }

    // Set up the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(1);
    }

    // Send the message to the server
    if (sendto(send_socket, message, sizeof(message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Send failed");
        exit(1);
    }
    printf("Message sent to server: %s\n", message);

    // Create the receiving socket
    recv_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (recv_socket < 0) {
        perror("Receive socket creation failed");
        exit(1);
    }

    // Bind the receiving socket to any available port
    memset(&recv_addr, 0, sizeof(recv_addr));
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_port = 0;  // Let the system pick any available port
    recv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(recv_socket, (struct sockaddr *)&recv_addr, sizeof(recv_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    // Get the port assigned to the receiving socket
    socklen_t recv_len = sizeof(recv_addr);
    if (getsockname(recv_socket, (struct sockaddr *)&recv_addr, &recv_len) == -1) {
        perror("Getsockname failed");
        exit(1);
    }

    printf("Listening for messages on port %d\n", ntohs(recv_addr.sin_port));

    // Listen for a message from the server
    ssize_t len = recvfrom(recv_socket, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&recv_addr, &addr_len);
    if (len < 0) {
        perror("Receive failed");
        exit(1);
    }

    // Null-terminate the received message and print it
    buffer[len] = '\0';
    printf("Received message from server: %s\n", buffer);

    // Clean up and close sockets
    close(send_socket);
    close(recv_socket);

    return 0;
}