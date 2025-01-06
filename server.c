#include "./include/connection.h"
#include "./include/service.h"
#include "include/checker.h"
#include "include/global.h"
#include "include/helper.h"
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    // cleanup allocated memory for distinct exiting signal
    signal(SIGINT, handle_sig);
    signal(SIGHUP, handle_sig);
    signal(SIGTERM, handle_sig);
    signal(SIGSEGV, handle_sig);

    if (argc != 2) return -1;

    if (strncmp(argv[1], "-i", strlen(argv[1])) == 0)
    {
        interactive_mode();
    }
    else if (is_valid_ip_port(argv[1], PORT))
    {
        int port = atoi(argv[1]);
        // create a socket for IPv6
        int sockfd = socket(AF_INET6, SOCK_STREAM, 0);

        if (sockfd < 0)
        {
            perror("Error opening socket");
        }

        // erver address setup
        struct sockaddr_in6 serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin6_family = AF_INET6;
        serv_addr.sin6_addr = in6addr_any;
        serv_addr.sin6_port = htons(port);

        // find the socket to the specified port
        if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            perror("Error on binding");
        }

        // listen for incoming connections
        listen(sockfd, 5);

        while (1)
        {
            pthread_t server_thread;
            pthread_attr_t pthread_attr;
            int *newsockfd = malloc(sizeof(int));
            struct sockaddr_in6 cli_addr;
            socklen_t clilen = sizeof(cli_addr);

            // accept a client connection
            *newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
            if (*newsockfd < 0)
            {
                perror("Error on accept");
            }

            // initialise and configure the thread attributes
            if (pthread_attr_init(&pthread_attr) != 0)
            {
                fprintf(stderr, "Error initialising thread attributes\n");
                return -1;
            }

            if (pthread_attr_setdetachstate(&pthread_attr,
                                            PTHREAD_CREATE_DETACHED) != 0)
            {
                fprintf(stderr, "Error setting thread attributes\n");
                return -1;
            }

            // create a new thread to handle the client request
            if (pthread_create(&server_thread, &pthread_attr, process_request,
                               (void *)newsockfd) != 0)
            {
                fprintf(stderr, "Error creating thread\n");
                return -1;
            }
        }
    }
    return 0;
}
