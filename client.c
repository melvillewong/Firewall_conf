#include "./include/connection.h"
#include "./include/global.h"
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <server_ip> <port>\n", argv[0]);
        return -1;
    }

    // parse IP and port from CL arguments
    int server_port = atoi(argv[2]);

    // create a TCP socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error opening socket");
        return -1;
    }

    // set up server address structure
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);

    // connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
        0)
    {
        perror("Error connecting to the server\n");
        close(sockfd);
        return -1;
    }

    // loop to send user commands to the server
    char buffer[BUFFER_SIZE] = "";
    for (int i = 3; i < argc; i++)
    {
        strncat(buffer, argv[i], BUFFER_SIZE - strlen(buffer) - 1);
        if (i < argc - 1)
        {
            strncat(buffer, " ", BUFFER_SIZE - strlen(buffer) - 1);
        }
    }

    if (strlen(buffer) != 0)
    {
        send_request(sockfd, buffer);
    }

    // close the socket
    close(sockfd);
    return 0;
}

void send_request(int sockfd, const char *msg)
{
    size_t msg_len = strlen(msg) + 1;

    // send the length of the message
    if (write(sockfd, &msg_len, sizeof(size_t)) < 0)
    {
        perror("Error writing message length to socket");
        return;
    }

    // send the actual message
    if (write(sockfd, msg, msg_len) < 0)
    {
        perror("Error writing message to socket");
        return;
    }

    // read the response length first
    size_t response_len;
    if (read(sockfd, &response_len, sizeof(size_t)) != sizeof(size_t))
    {
        perror("Error reading response length from socket");
        return;
    }

    // allocate buffer for the response based on the received length
    char *response = (char *)malloc(response_len);
    if (!response)
    {
        perror("Memory allocation failed");
        return;
    }

    // read the actual repsonse into the allocated buffer
    if (read(sockfd, response, response_len) < 0)
    {
        perror("Error reading response from server");
        free(response);
        return;
    }

    printf("%s\n", response);
    free(response);
}
