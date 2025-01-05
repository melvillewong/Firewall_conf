#ifndef CHECKER_H
#define CHECKER_H

#include "global.h"
int check_ip(const char *ips);
int check_port(const char *ports);
int check_ip_range(char *ip1, char *ip2);
int check_port_range(char *port1, char *port2);
int is_valid_ip_port(char *value, enum Network ip_or_port);
int is_ip_in_rule(char *rule_ips, const char *connect_ip);
int is_port_in_rule(char *rule_ports, const char *connect_port);

#endif
