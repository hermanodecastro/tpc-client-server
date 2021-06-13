#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

#define LISTEN_BACKLOG 2
#define BUFFER_SIZE 1024

int main(int argc, char const *argv[])
{
    if(argc != 2)
    {
        printf("Too few arguments, missing port\n");
        return 1;
    }

    int port = atoi(argv[1]);
    int socketFd = socket(AF_INET, SOCK_STREAM, 0);

    if(socketFd < 0)
    {
        perror("bind ");
        fprintf(stderr, "ERROR couldn't create socket\n");
        return 1;
    }

    struct sockaddr_in server;

    memset(&server, 0, sizeof(server));

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if(bind(socketFd, (struct sockaddr*)&server, sizeof(server)) < 0)
    {
        perror("bind ");
        fprintf(stderr, "ERROR binding socket\n");
        return 1;
    }

    if(listen(socketFd, LISTEN_BACKLOG) < 0)
    {
        perror("listen ");
        fprintf(stderr, "ERROR listen connections\n");
    }

    struct sockaddr_in client;
    socklen_t size = sizeof(struct sockaddr_in);

    printf("Waiting for connections...\n\n");

    while(1)
    {
        int pid = fork();

        int newSocket = accept(socketFd, (struct sockaddr*) &client, &size);

        if(newSocket < 0)
        {
            perror("accept ");
            fprintf(stderr, "ERROR accept\n");
            return 1;
        }

        printf("Connection established: %s:%d\n", inet_ntoa(client.sin_addr), client.sin_port);

        char message[BUFFER_SIZE];
        memset(&message, 0, sizeof(message));
        
        while(recv(newSocket, message, sizeof(message), 0) > 0)
        {
            printf("Received: %s\n", message);
            memset(&message, 0, sizeof(message));
        }
    } 
} 
