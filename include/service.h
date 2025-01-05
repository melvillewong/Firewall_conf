#ifndef SERVER_H
#define SERVER_H

void list_requests_cmd();
int add_rule_cmd(char *rule);
int connect_rule_cmd(char *ip_port);
int del_rule_cmd(char *rule);
void list_rule_cmd();

char *process_cmd(char *request);
void interactive_mode();

#endif
