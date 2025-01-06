#include "../include/connection.h"
#include "../include/service.h"
#include <netinet/in.h>
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// send a response to the client
int write_result(int sockfd, const char *buffer, size_t buffer_size)
{
    size_t bytes_written = 0;
    ssize_t n;

    // send response length
    if (write(sockfd, &buffer_size, sizeof(size_t)) < 0)
    {
        perror("Error writing response size to socket\n");
        return -1;
    }

    // send actual response
    while (bytes_written < buffer_size)
    {
        n = write(sockfd, buffer + bytes_written, buffer_size - bytes_written);
        if (n < 0)
        {
            perror("Error writing data to socket\n");
        }
        bytes_written += n;
    }

    return 0;
}

// read a meg from the client
char *read_request(int sockfd)
{
    size_t buffer_size;
    ssize_t res;
    char *buffer;

    // read the length of the message
    res = read(sockfd, &buffer_size, sizeof(size_t));
    if (res != sizeof(size_t))
    {
        perror("Error reading message size from socket\n");
        return NULL;
    }

    // allocate memory for the msg
    buffer = malloc(buffer_size + 1);
    if (!buffer)
    {
        perror("Memory allocation failed\n");
        return NULL;
    }
    buffer[buffer_size] = '\0'; // Null-terminate the msg

    // read the actual msg from the client
    size_t bytes_read = 0;
    while (bytes_read < buffer_size)
    {
        res = read(sockfd, buffer + bytes_read, buffer_size - bytes_read);
        if (res <= 0)
        {
            perror("Error reading msg from socket");
            free(buffer);
            return NULL;
        }
        bytes_read += res;
    }

    return buffer;
}

void *process_request(void *args)
{
    int *newsockfd = (int *)args;
    char *buffer = read_request(*newsockfd); // read te client's request
    char *result;

    if (!buffer)
    {
        fprintf(stderr, "Error reading from socket\n");
    }
    else
    {
        // process cmd
        result = process_cmd(buffer);

        // send the result back to the client
        if (result)
        {
            write_result(*newsockfd, result, strlen(result) + 1);
        }
    }

    close(*newsockfd);
    free(newsockfd);
    pthread_exit(NULL);
}
