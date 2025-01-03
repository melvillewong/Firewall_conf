#ifndef SERVER_H
#define SERVER_H

#define BUFFERLENGTH 256

int check_ip(const char *ip);
int check_port(const char *port);
char *process_cmd(char *request);
void interactive_mode();
void cleanup(int sig);

#endif
