#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 1024
#define PORT 20018 // Replace with the desired port number
#define SERVER_ADDR "10.100.23.204"

pthread_mutex_t mutexx;

int sockfd;
struct sockaddr_in serverAddr, clientAddr;
char buffer[BUFFER_SIZE];
socklen_t addrLen = sizeof(clientAddr);

const char *message = "18";

void* send_msg(void*) {
    pthread_mutex_lock(&mutexx);

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    serverAddr.sin_port = htons(PORT);

    if (sendto(sockfd, message, strlen(message), 0, (const struct sockaddr *)&serverAddr, addrLen) < 0) {
        perror("Sendto failed");
        close(sockfd);
        pthread_mutex_unlock(&mutexx);
        exit(EXIT_FAILURE);
    }

    pthread_mutex_unlock(&mutexx);
    printf("Message sent.\n");
}

void* receive_msg(void*) {
    pthread_mutex_lock(&mutexx);

    printf("Listening for messages from %s on port %d\n", SERVER_ADDR, PORT);
    printf("a");

    while(1) {
        printf("q");
        memset(buffer, 0, BUFFER_SIZE);
        if (recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&serverAddr, &addrLen) < 0) {
            perror("Recvfrom failed.\n");
            close(sockfd);
            pthread_mutex_unlock(&mutexx);
            exit(EXIT_FAILURE);
        }
        printf("Message received from server: %s\n", buffer);

        nanosleep(&(struct timespec){0, 1*1000*1000}, NULL);
    }

    pthread_mutex_unlock(&mutexx);
}

int main() {
    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed.\n");
        exit(EXIT_FAILURE);
    }

    pthread_t thread_send;
    pthread_t thread_receive;

    pthread_mutex_init(&mutexx, NULL);

    pthread_create(&thread_send, NULL, send_msg, NULL);
    pthread_create(&thread_receive, NULL, receive_msg, NULL);

    pthread_join(thread_send, NULL);
    pthread_join(thread_receive, NULL);

    // Close the socket (never reached in this example)
    close(sockfd);
    return 0;
}