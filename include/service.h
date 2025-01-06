#ifndef SERVER_H
#define SERVER_H

#include "global.h"

void list_requests_cmd(char *result);
int add_rule_cmd(char *rule);
Connection_t connect_rule_cmd(char *ip_port);
Deleteion_t del_rule_cmd(char *rule);
void list_rule_cmd(char *result);

char *process_cmd(char *request);
void interactive_mode();

#endif
