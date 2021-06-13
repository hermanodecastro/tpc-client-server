#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MESSAGE_SIZE 1024

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

    struct sockaddr_in serverAddress;

    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);

    if(connect(socketFd, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) < 0)
    {
        perror("connect ");
        fprintf(stderr, "ERROR couldn't connect\n");
        return 1;
    }

    char message[MESSAGE_SIZE];

    system("clear");
    printf("Enter [quit] to exit\n\n");

    while(strcmp(message, "quit") != 0)
    {
        memset(message, 0, sizeof(message));

        printf("> ");
        fgets(message, MESSAGE_SIZE, stdin);

        message[strcspn(message, "\n")] = 0;

        send(socketFd, message, strlen(message), 0);
    }

    close(socketFd);
}
