#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <assert.h>

#define SERVER_IP "10.100.23.204" // IP address of the server
#define SERVER_PORT 20025 // Port number of the server
#define BUF_SIZE 1024 // Buffer size
#define MSG_LENGTH 512

void* receive_msg() {
    int sockfd;
    struct sockaddr_in server_addr, other_addr;
    char message[BUF_SIZE];
    char buffer[BUF_SIZE];
    socklen_t addr_len = sizeof(other_addr);

    // Create a socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Required option because port Length = 74, Data: <You said: hrecently used
    int optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    // Bind the port
    int res = bind(sockfd,(struct sockaddr *) &server_addr, sizeof(server_addr));
    if(res == -1) perror("thr_udpListen:bind");

    while(1) {
        res = recvfrom(sockfd, buffer, BUF_SIZE, 0,(struct sockaddr *) &other_addr, &addr_len);
        if(res == -1) perror("thr_udpListen:recvfrom");
        if(res >= BUF_SIZE-1){
            fprintf(stderr, "recvfrom: Hmm, length of received message is larger than max message length: %d vs %d\n\n", res, BUF_SIZE);
            assert(res < BUF_SIZE-1);
        }
        printf("Received packet from %s:%d\nLength = %d, Data: <%s>\n\n", inet_ntoa(other_addr.sin_addr), ntohs(other_addr.sin_port), res, buffer);
    }

    close(sockfd);
}

void* send_msg() {
    char message[MSG_LENGTH];
    printf("Enter a message: ");
    fgets(message, MSG_LENGTH, stdin);
    
    // Remove the newline char from the input
    message[strlen(message) - 1] = '\0';

    struct sockaddr_in si_other;
    int s, slen = sizeof(si_other);

    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) perror("udp_send: socket");

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(SERVER_PORT);
    int res = inet_aton(SERVER_IP, &si_other.sin_addr);
    if(res==0) perror("inet_aton() failed\n");

    res = sendto(s, message, MSG_LENGTH, 0, (struct sockaddr *) &si_other, slen);
    if(res==-1) perror("udp_send: sendto()");

    close(s);
}

int main() {

    send_msg();
    receive_msg();

    return 0;
}