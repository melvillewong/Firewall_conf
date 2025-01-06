#include <stdlib.h>

#ifndef CONNECTION_H
#define CONNECTION_H

int write_result(int sockfd, const char *bffer, size_t buffer_size);
char *read_request(int sockfd);
void *process_request(void *args);

void send_request(int sockfd, const char *msg);

#endif
